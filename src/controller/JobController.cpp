#include "controller/JobController.h"
#include <nlohmann/json.hpp>

#include "exceptions/InvalidScheduleException.h"
#include "exceptions/JobNotFoundException.h"

using json = nlohmann::json;

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


crow::response JobController::_create_job(const crow::request &req) {

    try {
        json body = json::parse(req.body);
        JobData data;
        data._name             = body["name"];
        data._type             = schedule_type_from_string(body["type"]);
        data._command          = body["command"];
        data._schedule_payload = body["schedule_payload"].dump();

        JobData result = _service->create_job(data);

        json res;
        res["id"]               = result._id;
        res["name"]             = result._name;
        res["type"]             = schedule_type_to_string(result._type);
        res["status"]           = job_status_to_string(result._status);
        res["next_run"]         = result._next_run.time_since_epoch().count();
        res["schedule_payload"] = result._schedule_payload;

        return crow::response(201, res.dump());
    }
    catch (JobNotFoundException& e) {
        json res = {
            {"status", 404},
            {"message", e.what()}
        };
        return crow::response(404, res.dump());

    }
    catch (InvalidScheduleException& e) {
        json res = {
            {"status", 400},
            {"message", e.what()}
        };
        return crow::response(400, res.dump());
    }
    catch (std::runtime_error& e) {
        json res = {
            {"status", 400},
            {"message", e.what()}
        };
        return crow::response(400, res.dump());
    }
    catch (std::exception& e) {
        json res = {
            {"status", 500},
            {"message", e.what()}
        };
        return crow::response(500, res.dump());
    }
}

crow::response JobController::_pause_job(const crow::request &req, const std::string& id) {

    try {
        JobData result = _service->pause_job(id);

        json res;
        res["id"]               = result._id;
        res["name"]             = result._name;
        res["type"]             = schedule_type_to_string(result._type);
        res["status"]           = job_status_to_string(result._status);
        res["next_run"]         = result._next_run.time_since_epoch().count();
        res["schedule_payload"] = result._schedule_payload;

        return crow::response(200, res.dump());
    }
    catch (JobNotFoundException& e) {
        json res = {
            {"status", 404},
            {"message", e.what()}
        };
        return crow::response(404, res.dump());

    }
    catch (std::runtime_error& e) {
        json res = {
            {"status", 400},
            {"message", e.what()}
        };
        return crow::response(400, res.dump());
    }
    catch (std::exception& e) {
        json res = {
            {"status", 500},
            {"message", e.what()}
        };
        return crow::response(500, res.dump());
    }
}


