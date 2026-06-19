#include "scheduler/ThreadPool.h"


ThreadPool::ThreadPool(const size_t num_threads) {
    for (size_t i = 0; i < num_threads; i++) {
        _workers.emplace_back(&ThreadPool::_worker_loop, this);
    }
}

ThreadPool::~ThreadPool() {
    stop();
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