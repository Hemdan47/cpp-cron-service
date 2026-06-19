#include "controller/JobController.h"
#include <nlohmann/json.hpp>

#include "exceptions/InvalidScheduleException.h"
#include "exceptions/JobNotFoundException.h"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

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
    CROW_ROUTE((*_app), "/api/jobs/<string>/pause").methods(crow::HTTPMethod::PATCH)
    ([this](const crow::request& req , const std::string& id) {
        return _pause_job(req, id);
    });

    //resume a job
    CROW_ROUTE((*_app), "/api/jobs/<string>/resume").methods(crow::HTTPMethod::PATCH)
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

        ordered_json res_json;
        res_json["id"]               = result._id;
        res_json["name"]             = result._name;
        res_json["type"]             = schedule_type_to_string(result._type);
        res_json["status"]           = job_status_to_string(result._status);
        res_json["next_run"]         = result._next_run.time_since_epoch().count();
        res_json["command"]          = result._command;
        res_json["schedule_payload"] = json::parse(result._schedule_payload);

        crow::response res(201, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (JobNotFoundException& e) {
        ordered_json res_json = {
            {"status", 404},
            {"message", e.what()}
        };
        crow::response res(404, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;

    }
    catch (InvalidScheduleException& e) {
        ordered_json res_json = {
            {"status", 400},
            {"message", e.what()}
        };
        crow::response res(400, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (std::runtime_error& e) {
        ordered_json res_json = {
            {"status", 400},
            {"message", e.what()}
        };
        crow::response res(400, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (std::exception& e) {
        ordered_json res_json = {
            {"status", 500},
            {"message", e.what()}
        };
        crow::response res(500, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
}

crow::response JobController::_pause_job(const crow::request &req, const std::string& id) {

    try {
        JobData result = _service->pause_job(id);

        ordered_json res_json;
        res_json["id"]               = result._id;
        res_json["name"]             = result._name;
        res_json["type"]             = schedule_type_to_string(result._type);
        res_json["status"]           = job_status_to_string(result._status);
        res_json["next_run"]         = result._next_run.time_since_epoch().count();
        res_json["command"]          = result._command;
        res_json["schedule_payload"] = json::parse(result._schedule_payload);

        crow::response res(200, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (JobNotFoundException& e) {
        ordered_json res_json = {
            {"status", 404},
            {"message", e.what()}
        };
        crow::response res(404, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;

    }
    catch (std::runtime_error& e) {
        ordered_json res_json = {
            {"status", 400},
            {"message", e.what()}
        };
        crow::response res(400, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (std::exception& e) {
        ordered_json res_json = {
            {"status", 500},
            {"message", e.what()}
        };
        crow::response res(500, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
}

crow::response JobController::_resume_job(const crow::request &req, const std::string& id) {

    try {
        JobData result = _service->resume_job(id);

        ordered_json res_json;
        res_json["id"]               = result._id;
        res_json["name"]             = result._name;
        res_json["type"]             = schedule_type_to_string(result._type);
        res_json["status"]           = job_status_to_string(result._status);
        res_json["next_run"]         = result._next_run.time_since_epoch().count();
        res_json["command"]          = result._command;
        res_json["schedule_payload"] = json::parse(result._schedule_payload);

        crow::response res(200, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (JobNotFoundException& e) {
        ordered_json res_json = {
            {"status", 404},
            {"message", e.what()}
        };
        crow::response res(404, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;

    }
    catch (std::runtime_error& e) {
        ordered_json res_json = {
            {"status", 400},
            {"message", e.what()}
        };
        crow::response res(400, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (std::exception& e) {
        ordered_json res_json = {
            {"status", 500},
            {"message", e.what()}
        };
        crow::response res(500, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
}

crow::response JobController::_delete_job(const crow::request &req, const std::string& id) {

    try {
        _service->delete_job(id);
        ordered_json res_json;
        res_json["id"] = id;
        res_json["message"] = "Job deleted successfully";

        crow::response res(200, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (JobNotFoundException& e) {
        ordered_json res_json = {
            {"status", 404},
            {"message", e.what()}
        };
        crow::response res(404, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;

    }
    catch (std::runtime_error& e) {
        ordered_json res_json = {
            {"status", 400},
            {"message", e.what()}
        };
        crow::response res(400, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (std::exception& e) {
        ordered_json res_json = {
            {"status", 500},
            {"message", e.what()}
        };
        crow::response res(500, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }

}
crow::response JobController::_get_job_by_id(const std::string& id) {

    try {
        JobData result = _service->get_job_by_id(id);

        ordered_json res_json;
        res_json["id"] = result._id;
        res_json["name"] = result._name;
        res_json["type"] = schedule_type_to_string(result._type);
        res_json["status"] = job_status_to_string(result._status);
        res_json["next_run"] = result._next_run.time_since_epoch().count();
        res_json["command"] = result._command;
        res_json["schedule_payload"] = json::parse(result._schedule_payload);

        crow::response res(200, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (JobNotFoundException& e) {
        ordered_json res_json = {
            {"status", 404},
            {"message", e.what()}
        };
        crow::response res(404, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (std::runtime_error& e) {
        ordered_json res_json = {
            {"status", 400},
            {"message", e.what()}
        };
        crow::response res(400, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (std::exception& e) {
        ordered_json res_json = {
            {"status", 500},
            {"message", e.what()}
        };
        crow::response res(500, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
}
crow::response JobController::_list_jobs() {

    try {
        std::vector<JobData> jobs = _service->get_all_jobs();

        ordered_json res_json = ordered_json::array();

        for (const auto& job : jobs) {
            res_json.push_back(ordered_json{
                {"id", job._id},
                {"name", job._name},
                {"type", schedule_type_to_string(job._type)},
                {"status", job_status_to_string(job._status)},
                {"next_run", job._next_run.time_since_epoch().count()},
                {"command", job._command},
                {"schedule_payload", json::parse(job._schedule_payload)}
            });
        }

        crow::response res(200, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (std::runtime_error& e) {
        ordered_json res_json = {
            {"status", 400},
            {"message", e.what()}
        };
        crow::response res(400, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
    catch (std::exception& e) {
        ordered_json res_json = {
            {"status", 500},
            {"message", e.what()}
        };
        crow::response res(500, res_json.dump());
        res.set_header("Content-Type", "application/json");
        return res;
    }
}
