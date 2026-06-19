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



