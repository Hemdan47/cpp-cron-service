#pragma once
#include <memory>
#include <crow.h>

#include "controller/JobController.h"
#include "factory/JobFactory.h"
#include "repository/IJobRepository.h"
#include "scheduler/SchedulerDaemon.h"
#include "service/JobRecoveryService.h"
#include "service/JobService.h"



class Application {
public:
    Application();
    ~Application() = default;

    void run();


private:
    std::shared_ptr<IJobRepository> _repository;
    std::shared_ptr<JobFactory> _factory;
    std::shared_ptr<SchedulerDaemon> _daemon;
    std::shared_ptr<ThreadPool> _thread_pool;
    std::shared_ptr<JobService> _service;
    std::shared_ptr<JobRecoveryService> _recovery_service;
    std::shared_ptr<JobController> _controller;
    std::shared_ptr<crow::SimpleApp> _crow;
};
