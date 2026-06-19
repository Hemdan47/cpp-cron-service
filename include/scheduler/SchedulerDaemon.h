#pragma once
#include <condition_variable>
#include <memory>
#include <queue>
#include <unordered_set>
#include "ThreadPool.h"
#include "domain/Job.h"

struct JobComparator {
    bool operator()(const std::shared_ptr<Job>& lhs, const std::shared_ptr<Job>& rhs) const {
        if (!lhs && !rhs) return false;
        if (!lhs) return true;
        if (!rhs) return false;
        return lhs->get_next_run() > rhs->get_next_run();
    }
};

class SchedulerDaemon {
public:
    explicit SchedulerDaemon(std::shared_ptr<ThreadPool> _thread_pool);
    ~SchedulerDaemon();

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
    std::priority_queue<std::shared_ptr<Job>, std::vector<std::shared_ptr<Job>>, JobComparator> _queue;
    std::unordered_set<std::string> _active_jobs;
};
