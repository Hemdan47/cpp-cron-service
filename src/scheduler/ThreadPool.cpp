#include "scheduler/ThreadPool.h"
#include "service/JobService.h"

#include <iostream>
#include <boost/process.hpp>
#include "exceptions/JobExecutionException.h"


ThreadPool::ThreadPool(const size_t num_threads) {
    for (size_t i = 0; i < num_threads; i++) {
        _workers.emplace_back(&ThreadPool::_worker_loop, this);
    }
}

ThreadPool::~ThreadPool() {
    stop();
}

void ThreadPool::set_service(std::shared_ptr<JobService> service) {
    this->_service = std::move(service);
}


void ThreadPool::stop() {
    _stop_signal = true;
    _cv.notify_all();
    for (auto& worker : _workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}


void ThreadPool::enqueue(std::shared_ptr<Job> task) {
    std::lock_guard<std::mutex> lock(_mutex);
    _tasks.push(std::move(task));
    _cv.notify_one();
}

void ThreadPool::_worker_loop() {
    while (!_stop_signal) {
        std::unique_lock<std::mutex> lock(_mutex);

        _cv.wait(lock, [this] {
            return !_tasks.empty() || _stop_signal;
        });

        if (_stop_signal && _tasks.empty()) return;

        auto job = _tasks.front();
        _tasks.pop();
        lock.unlock();

        _execute(job);
    }
}

void ThreadPool::_execute(std::shared_ptr<Job> job) {
    const std::string id = job->get_id();
    try {
        _service->update_status(id, JobStatus::RUNNING);

        boost::process::child process(job->get_command());
        process.wait();

        const int exit_code = process.exit_code();

        if (exit_code != 0) {
            throw JobExecutionException(id,
                "process exited with code " + std::to_string(exit_code));
        }

        //success path
        if (job->get_schedule_type() == ScheduleType::ONETIME) {
            _service->update_status(id, JobStatus::COMPLETED);
        } else {
            // this set the status to active and recalculate the next next run and add to the daemon
            _service->reschedule_job(id);
        }
    }
    catch (const JobExecutionException& e) {
        try {
            _service->update_status(id, JobStatus::FAILED);
        }
        catch (const std::exception& inner) {
            std::cerr << "Failed to mark job FAILED: " << inner.what() << "\n";
        }
    }
    catch (const std::exception& e) {
        // boost::process threw (command not found, permission denied, etc.)
        // or the db threw on the initial RUNNING update
        try {
            _service->update_status(id, JobStatus::FAILED);
        }
        catch (const std::exception& inner) {
            std::cerr << "Failed to mark job FAILED: " << inner.what() << "\n";
        }
    }
}