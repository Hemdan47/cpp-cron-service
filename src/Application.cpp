#include "Application.h"
#include "repository/SQliteRepository.h"


Application::Application() {
    this->_repository = std::make_shared<SQliteRepository>();
    // this->_factory = std::make_shared<JobFactory>();
    // this->_daemon = std::make_shared<SchedulerDaemon>(_repo);
    // this->_service = std::make_shared<JobService>(repo, factory, daemon);
    // this->_controller = std::make_shared<JobController>(service);
}

void Application::run() {
    
    // JobRecoveryService recovery_manager(_repo, _factory, _daemon);
    // recovery_manager.execute();
    // _controller->registerRoutes(_crow);
    // _daemon->start();
    // _crow->port(8080).multithreaded().run();
}