#include "scheduler/SchedulerDaemon.h"

SchedulerDaemon::SchedulerDaemon(std::shared_ptr<ThreadPool> thread_pool)
    : _thread_pool(std::move(thread_pool)), _stop_signal(false){}

void SchedulerDaemon::start() {
    _thread = std::thread(&SchedulerDaemon::run, this);
}

void SchedulerDaemon::stop() {
    _stop_signal = true;
    _cv.notify_all();
    if (_thread.joinable()) {
        _thread.join();
    }
}

void SchedulerDaemon::add_job(std::shared_ptr<Job> job) {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push(job);
    _active_jobs.insert(job->get_id());
    _cv.notify_one();
}

void SchedulerDaemon::remove_job(const std::string& id) {
    //we can't remove a job from the priority queue in efficient time so we will soft delete by
    //removing the id from the active_jobs and double check in the run loop
    std::lock_guard<std::mutex> lock(_mutex);
    _active_jobs.erase(id);
}

