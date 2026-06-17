#include "controller/JobController.h"

JobController::JobController(std::shared_ptr<JobService> service, std::shared_ptr<crow::SimpleApp> app)
    :_service(std::move(service)), _app(std::move(app))
{
    _register_routes(app);
}


void JobController::_register_routes(std::shared_ptr<crow::SimpleApp> app) {
    //create a job
    CROW_ROUTE((*_app), "/api/jobs").methods(crow::HTTPMethod::POST)
    ([this](const crow::request& req) {
        return _create_job(req);
    });

    //pause a job
    CROW_ROUTE((*_app), "/api/jobs/<string>").methods(crow::HTTPMethod::PATCH)
    ([this](const crow::request& req , const std::string& id) {
        return _pause_job(req, id);
    });

    //resume a job
    CROW_ROUTE((*_app), "/api/jobs/<string>").methods(crow::HTTPMethod::PATCH)
    ([this](const crow::request& req , const std::string& id) {
        return _resume_job(req, id);
    });

    //delete a job
    CROW_ROUTE((*_app), "/api/jobs/<string>").methods(crow::HTTPMethod::Delete)
    ([this](const crow::request& req , const std::string& id) {
        return _delete_job(req, id);
    });

    //get job by id
    CROW_ROUTE((*_app), "/api/jobs/<string>").methods(crow::HTTPMethod::GET)
    ([this](const std::string& id) {
        return _get_job_by_id(id);
    });

    //get all jobs
    CROW_ROUTE((*_app), "/api/jobs").methods(crow::HTTPMethod::GET)
    ([this]() {
        return _list_jobs();
    });

}
