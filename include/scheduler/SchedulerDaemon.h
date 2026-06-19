#pragma once
#include <condition_variable>
#include <memory>
#include <queue>
#include <unordered_set>
#include "ThreadPool.h"
#include "domain/Job.h"

class SchedulerDaemon {
public:
    explicit SchedulerDaemon(std::shared_ptr<ThreadPool> _thread_pool);
    ~SchedulerDaemon() = default;

    void add_job(std::shared_ptr<Job> job);
    void remove_job(const std::string& id);
    void start();
    void stop();
    void run();

private:
    std::thread _thread;
    std::shared_ptr<ThreadPool> _thread_pool;
    std::condition_variable _cv;
    std::mutex _mutex;
    std::atomic<bool> _stop_signal;
    std::priority_queue<std::shared_ptr<Job>> _queue;
    std::unordered_set<std::string> _active_jobs;
};
