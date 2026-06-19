#include "Application.h"
#include "repository/SQliteRepository.h"
#include "service/JobRecoveryService.h"


Application::Application() {
    this->_repository = std::make_shared<SQliteRepository>();
    this->_factory = std::make_shared<JobFactory>();
    this->_thread_pool = std::make_shared<ThreadPool>();
    this->_daemon = std::make_shared<SchedulerDaemon>(_thread_pool);
    this->_service = std::make_shared<JobService>(_repository, _factory, _daemon);
    this->_recovery_service = std::make_unique<JobRecoveryService>(_repository, _factory, _daemon);
    this->_thread_pool->set_service(_service);
    this->_crow = std::make_shared<crow::SimpleApp>();
    this->_controller = std::make_shared<JobController>(_service , _crow);

}

void Application::run() {
    this->_daemon->start();
    this->_recovery_service->execute();
    
    this->_crow->port(8080).multithreaded().run();

    this->_daemon->stop();
    this->_thread_pool->stop();
}