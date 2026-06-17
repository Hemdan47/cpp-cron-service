#pragma once
#include "crow/app.h"
#include "service/JobService.h"


class JobController {
public:
    JobController(std::shared_ptr<JobService> service, std::shared_ptr<crow::SimpleApp> app);

private:
    std::shared_ptr<JobService> _service;
    std::shared_ptr<crow::SimpleApp> _app;

    void _register_routes(std::shared_ptr<crow::SimpleApp> app);
    crow::response _create_job(const crow::request &req);
    crow::response _pause_job(const crow::request &req, const std::string& id);
    crow::response _resume_job(const crow::request &req, const std::string& id);
    crow::response _delete_job(const crow::request &req, const std::string& id);
    crow::response _get_job_by_id(const std::string& id);
    crow::response _list_jobs();

};
