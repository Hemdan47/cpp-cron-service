#pragma once
#include <memory>
#include <crow.h>
#include "repository/IJobRepository.h"

class Application {
public:
    Application();
    ~Application() = default;

    void run();


private:
    std::shared_ptr<IJobRepository> _repository;
    // std::shared_ptr<JobFactory> _factory;
    // std::shared_ptr<SchedulerDaemon> _daemon;
    // std::shared_ptr<JobService> _service;
    // std::shared_ptr<JobController> _controller;
    std::unique_ptr<crow::SimpleApp> _crow;
};
