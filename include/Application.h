#pragma once
#include <memory>
#include <crow.h>
#include "domain/IJobRepository.h"

class Application {
public:
    Application();
    ~Application() = default;

    void run();


private:
    std::shared_ptr<IJobRepository> repo;
    // std::shared_ptr<JobFactory> factory;
    // std::shared_ptr<SchedulerDaemon> daemon;
    // std::shared_ptr<JobService> service;
    // std::shared_ptr<JobController> controller;
    std::unique_ptr<crow::SimpleApp> crow;
};