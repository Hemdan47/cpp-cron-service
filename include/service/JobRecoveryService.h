#pragma once
#include <memory>
#include "repository/IJobRepository.h"
#include "factory/JobFactory.h"
#include "scheduler/SchedulerDaemon.h"

class JobRecoveryService {
public:
    JobRecoveryService(
        std::shared_ptr<IJobRepository> repo,
        std::shared_ptr<JobFactory> factory,
        std::shared_ptr<SchedulerDaemon> daemon);

    void execute();

private:
    std::shared_ptr<IJobRepository>   _repo;
    std::shared_ptr<JobFactory>       _factory;
    std::shared_ptr<SchedulerDaemon>  _daemon;


};
