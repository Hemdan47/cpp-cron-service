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

