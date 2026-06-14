#pragma once
#include <memory>
#include <crow.h>

class Application {
public:
    Application();
    ~Application() = default;

    void run();


private:
    // std::shared_ptr<SQLiteRepo> repo;
    // std::shared_ptr<JobFactory> factory;
    // std::shared_ptr<SchedulerDaemon> daemon;
    // std::shared_ptr<JobService> service;
    // std::shared_ptr<JobController> controller;
    std::unique_ptr<crow::SimpleApp> crow;
};