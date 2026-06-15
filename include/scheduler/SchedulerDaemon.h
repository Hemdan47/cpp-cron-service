#pragma once
#include <memory>
#include <queue>
#include "domain/Job.h"

class SchedulerDaemon {
public:
    SchedulerDaemon() = default;
    ~SchedulerDaemon() = default;

    void add_job(std::shared_ptr<Job> job);
    void run();

private:
    std::priority_queue<std::shared_ptr<Job>> _queue;
};
