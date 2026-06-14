#include "Application.h"
#include "repository/SQliteRepository.h"


Application::Application() {
    this->repo = std::make_shared<SQliteRepository>();
    // this->factory = std::make_shared<JobFactory>();
    // this->daemon = std::make_shared<SchedulerDaemon>(repo);
    // this->service = std::make_shared<JobService>(repo, factory, daemon);
    // this->controller = std::make_shared<JobController>(service);
}

void Application::run() {
    // JobRecoveryService recoveryManager(repo, factory, daemon);
    // recoveryManager.execute();
    // controller->registerRoutes(crowApp);
    // daemon->start();
    // crow->port(8080).multithreaded().run();
}