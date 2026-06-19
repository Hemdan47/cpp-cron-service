# cpp-cron-service

A job scheduling service that runs shell commands on cron, interval, or
one-time schedules. Jobs are submitted over a REST API, persisted to SQLite, and
executed out of process by a fixed size worker pool.

## Summary

`cpp-cron-service` is a persistent task runner. A client POSTs a job (a shell
command plus a schedule) to the HTTP API. The service:

1. Validates and parses the schedule, computes the first run time.
2. Persists the job to SQLite.
3. Registers it with a scheduler daemon.
4. At the due time, the daemon hands the job to a worker pool, which spawns the
   command as a child process via `boost::process` and waits for it.
5. On success, recurring jobs are rescheduled; one-time jobs are marked
   completed. On failure, the job is marked failed.

State is durable: after a restart, active jobs are reloaded from the database, re-registered with the scheduler, and missed recurring jobs are reconciled.

## Dependencies

All dependencies are fetched automatically by CMake via `FetchContent` — no vcpkg/Conan needed.

- **Crow** — HTTP server / routing
- **Asio** (standalone) — async I/O backend for Crow
- **nlohmann/json** — JSON parsing and serialization
- **SQLiteCpp** — RAII C++ wrapper over SQLite3, used for persistence
- **Boost.Process** — child-process execution for running job commands

On Windows, `ws2_32` and `mswsock` are linked for Asio's socket layer.

## Requirements

- C++20 compiler (GCC 13+, Clang 16+, or MSVC 19.34+)
- CMake ≥ 3.16
- Internet access on first build (dependencies are fetched via `FetchContent`)

## Installation

```bash
cmake -S . -B build
cmake --build build
```

Run (creates/opens the SQLite database `job.db3` in the working directory):

```bash
./build/cpp-cron-service        # Linux/macOS
build\Release\cpp-cron-service.exe   # Windows
```

The server blocks on port 8080. Stop with `Ctrl+C`; on exit the daemon and pool
are joined cleanly.

### Notes

- The SQLite file is `job.db3` next to the executable; it is created on first
  run and the `jobs` table is created if missing.
- On Windows, `ws2_32`/`mswsock` are linked automatically for Asio.
- Commands are executed verbatim as child processes via the OS shell rules of
  `boost::process`; ensure the binaries you reference are on `PATH` or absolute.

## Architecture

Layered, with manual dependency injection in `Application`:

- **JobController** — Crow routes, JSON (de)serialization, HTTP status mapping.
- **JobService** — orchestrates repository + factory + daemon; lifecycle logic.
- **JobFactory** — maps `ScheduleType` → concrete `Job` subclass.
- **SchedulerDaemon** — owns one thread; decides *when* a job fires.
- **ThreadPool** — owns N threads; runs the command and reports outcome.
- **JobRecoveryService** — runs once at startup to rebuild the scheduler queue from the DB.

## Domain Model

`Job` is an abstract base holding `id, name, type, command, status, next_run,
schedule_payload`, with a pure-virtual `calculate_next_run()`. `JobData` is the
plain DTO carried across layers. `JobFactory` holds a
`std::map<ScheduleType, std::function<std::unique_ptr<Job>(const JobData&)>>`
registered with lambdas per type — classic factory with late lookup.

### Schedule types and payloads

| `type` | `schedule_payload` | `calculate_next_run()` semantics |
|---|---|---|
| `CRON` | `{"expression":"m h dom mon dow"}` | 5-field cron; each field expands to a `std::set<int>` of allowed values |
| `INTERVAL` | `{"interval_seconds": N}` | fire every N seconds; catches up after downtime by skipping missed cycles |
| `ONETIME` | `{"iso_date":"YYYY-MM-DDTHH:MM:SS"}` | fires once at the given instant |

Cron field grammar supported by `_parse_field`: `*` (full range), `a-b` (inclusive range), or a single integer.

`IntervalJob` computes catch-up by dividing elapsed time by the interval: `next_run += (missed + 1) * interval`, so it does not fire a burst of missed runs, just resumes at the next due slot.

Time is handled throughout with C++20 `<chrono>` calendrical types
(`sys_seconds`, `year_month_day`, `weekday`, `hh_mm_ss`); `next_run` is stored
in SQLite as an `int64` epoch-seconds count.

## Scheduling Internals (`SchedulerDaemon`)

The daemon is a single dedicated background thread backed by a `std::priority_queue` of
`shared_ptr<Job>` keyed on `next_run`, plus an `std::unordered_set<std::string>
_active_jobs` that tracks live ids (used for soft-delete, see below).

The run loop (`SchedulerDaemon::run`):

```
while (!stop) {
    lock mutex
    if queue empty: _cv.wait(lock)        // sleep until add_job() notifies
    job  = queue.top()                    // nearest deadline
    next = job->get_next_run()
    status = _cv.wait_until(lock, next)   // release lock, sleep until deadline

    if status == timeout:
        queue.pop()
        if _active_jobs.contains(job->id):
            _active_jobs.erase(job->id)
            thread_pool->enqueue(job)     // dispatch for execution
}
```

Two properties fall out of this:

- **Single wake-up source.** The daemon sleeps on one condition variable.
  `wait_until` either times out at the deadline (dispatch the top job) or is
  interrupted by `add_job()`'s `notify_one` (loop, re-peek the top). That
  means inserting a job with an earlier deadline than the currently-slept one
  preempts the current sleep, so the nearest deadline is always what it waits on.
- **Soft delete.** A `std::priority_queue` has no efficient erase, so
  `remove_job(id)` only erases the id from `_active_jobs`. When a popped job's
  id is no longer in the set (it was paused/deleted), the dispatch is skipped.
  This is how pause and delete take effect on already-queued jobs.

`add_job` pushes the job, inserts its id into `_active_jobs`, and notifies. The
daemon thread is started by `start()` (spawned from `Application::run`) and torn
down by `stop()`, which flips the `_stop_signal` atomic, notifies the cv, and
joins the thread.

Recurring re-registration: the daemon itself does **not** reschedule a recurring
job. After a worker finishes it calls `JobService::reschedule_job(id)`, which
recalculates `next_run`, persists it, and calls `daemon->add_job()` again — so a
recurring job re-enters the queue only after its previous run completes.

## Multithreading & Execution (`ThreadPool`)

The pool owns a fixed number of worker threads (`std::thread::hardware_concurrency()`)
and a plain FIFO `std::queue<shared_ptr<Job>> _tasks`.

Each worker (`_worker_loop`):

```
while (!stop) {
    unique_lock lock(mutex)
    _cv.wait(lock, []{ return !tasks.empty() || stop; })   // predicate-guarded
    if stop && tasks.empty(): return
    job = tasks.front(); tasks.pop()
    unlock
    _execute(job)
}
```

`enqueue` locks, pushes, and `notify_one` (waking exactly one idle worker).
`stop` flips `_stop_signal`, notifies **all** workers, and joins each. The
wait predicate lets a worker exit cleanly during shutdown.

### `_execute` — the command lifecycle

```
update_status(id, RUNNING)                      // via JobService → DB
child = boost::process::child(command)
child.wait()                                    // BLOCKS this worker until exit
exit_code = child.exit_code()
if exit_code != 0: throw JobExecutionException(...)
if type == ONETIME: update_status(id, COMPLETED)
else:               reschedule_job(id)          // recalc next_run, persist, re-add to daemon
on any exception:   update_status(id, FAILED)   // (swallows inner DB errors to stderr)
```

Key points:

- **One job per worker for its full duration.** `child.wait()` is synchronous, so
  while a command runs that worker is busy and not pulling from the queue.
  Parallelism is bounded by pool size; if all workers are busy, new due jobs
  queue in the pool until one frees up.
- **Out-of-process execution.** Commands are real OS child processes, fully
  isolated from the service. Non-zero exit is treated as failure.
- **Service callback.** The pool calls back into `JobService` to flip status and
  to reschedule. Because the service touches the shared daemon and repository,
  those components use their own internal locking — the daemon mutex guards the
  scheduler queue; the worker only holds the pool mutex while touching `_tasks`.

## Startup Recovery (`JobRecoveryService`)

On `Application::run`,  `JobRecoveryService::execute()`
loads all `ACTIVE` rows and, for each, compares the stored `next_run` to `now`:

- `next_run` already in the future → re-add to the daemon as-is.
- `next_run` in the past and `ONETIME` → the window passed while down, so mark `COMPLETED` and skip.
- `next_run` in the past and recurring → call `calculate_next_run()` (which
  advances to the next valid slot), persist, then re-add.

This makes job state survive restarts without firing a backlog of missed runs.

## REST API

Crow app listens on **port 8080** in multithreaded mode. All bodies are JSON;
errors return `{"status": code, "message": ...}`.

| Method | Path | Action |
|---|---|---|
| `POST` | `/api/jobs` | Create + schedule a job |
| `GET` | `/api/jobs` | List all jobs |
| `GET` | `/api/jobs/<id>` | Get one job |
| `PATCH` | `/api/jobs/<id>/pause` | Pause (removes from daemon, status `PAUSED`) |
| `PATCH` | `/api/jobs/<id>/resume` | Resume (recalculates next run, re-adds) |
| `DELETE` | `/api/jobs/<id>` | Delete permanently |

Status code mapping: `404` `JobNotFoundException`, `400` `InvalidScheduleException` / `runtime_error`, `500` any other `std::exception`. `201` on create, `200` otherwise.

### Create request

`schedule_payload` is sent as a JSON object and stored serialized:

```bash
# Cron: 14:30 every weekday
curl -X POST http://localhost:8080/api/jobs \
  -H "Content-Type: application/json" \
  -d '{"name":"daily-report","type":"CRON",
       "command":"echo hello >> report.log",
       "schedule_payload":{"expression":"30 14 * * 1-5"}}'

# Interval: every 60 seconds
curl -X POST http://localhost:8080/api/jobs \
  -H "Content-Type: application/json" \
  -d '{"name":"poller","type":"INTERVAL",
       "command":"curl -s http://example.com/health",
       "schedule_payload":{"interval_seconds":60}}'

# One-time: fire once at a fixed instant
curl -X POST http://localhost:8080/api/jobs \
  -H "Content-Type: application/json" \
  -d '{"name":"one-shot","type":"ONETIME",
       "command":"backup.sh",
       "schedule_payload":{"iso_date":"2026-07-01T15:30:00"}}'
```

### Response shape

```json
{
  "id": "1718803200000_44721",
  "name": "daily-report",
  "type": "CRON",
  "status": "ACTIVE",
  "next_run": 1718806200,
  "schedule_payload": {
    "expression": "30 14 * * 1-5"
  }
}
```

`next_run` is the epoch-second count of the next scheduled execution


## License

This project is open source and available under the [MIT License](LICENSE).