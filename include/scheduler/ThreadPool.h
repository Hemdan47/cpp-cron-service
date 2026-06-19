#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>
#include "domain/Job.h"

class JobService;

class ThreadPool {
public:
    explicit ThreadPool(const size_t num_threads = std::thread::hardware_concurrency());
    ~ThreadPool();

    void enqueue(std::shared_ptr<Job> job);
    void stop();
    void set_service(std::shared_ptr<JobService> service);

private:
    void _worker_loop();
    void _execute(std::shared_ptr<Job> job);

    std::vector<std::thread> _workers;
    std::queue<std::shared_ptr<Job>> _tasks;
    std::mutex _mutex;
    std::condition_variable _cv;
    std::atomic<bool> _stop_signal;
    std::shared_ptr<JobService> _service;
};
