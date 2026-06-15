#include <format>
#include <iostream>
#include <optional>
#include <repository/SQliteRepository.h>
#include "SQLiteCpp/Database.h"
#include "domain/JobStatus.h"
#include "domain/ScheduleType.h"

SQliteRepository::SQliteRepository() {
    try {
        this->db = std::make_unique<SQLite::Database>("job.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
        this->initialize_schema();

    }
    catch (std::exception& e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

void SQliteRepository::initialize_schema() const{
    db->exec(R"(
            CREATE TABLE IF NOT EXISTS jobs (
                id               TEXT PRIMARY KEY,
                name             TEXT NOT NULL,
                type             TEXT NOT NULL,
                status           TEXT NOT NULL,
                command           TEXT NOT NULL,
                next_run         INTEGER,
                schedule_payload TEXT
            )
        )");
}

JobData SQliteRepository::save_job(const JobData& data) {

    const std::int64_t next_run = data._next_run.time_since_epoch().count();
    const std::string scheduler_type = schedule_type_to_string(data._type);
    const std::string scheduler_status = job_status_to_string(data._status);

    SQLite::Statement statement(*db, "INSERT INTO jobs VALUES(?, ?, ?, ?, ?, ?, ?)");

    statement.bind(1, data._id);
    statement.bind(2, data._name);
    statement.bind(3, scheduler_type);
    statement.bind(4, scheduler_status);
    statement.bind(5, data._command);
    statement.bind(6, next_run);
    statement.bind(7, data._schedule_payload);

    const int count = statement.exec();
    if (count == 0) {
        throw std::runtime_error("can't save the job");
    }

    auto saved_job = find_by_id(data._id);
    return saved_job.value();

}

JobData SQliteRepository::update_job(const JobData& data) {

    const std::int64_t next_run = data._next_run.time_since_epoch().count();
    const std::string scheduler_type = schedule_type_to_string(data._type);
    const std::string scheduler_status = job_status_to_string(data._status);

    SQLite::Statement statement(*db,
        "UPDATE jobs SET name = ?, type = ?, status = ?, command = ?, next_run = ?, schedule_payload = ? WHERE id = ?");

    statement.bind(1, data._name);
    statement.bind(2, scheduler_type);
    statement.bind(3, scheduler_status);
    statement.bind(4, data._command);
    statement.bind(5, next_run);
    statement.bind(6, data._schedule_payload);
    statement.bind(7, data._id);

    const int count = statement.exec();
    if (count == 0) {
        throw std::runtime_error("can't update the job");
    }

    auto updated_job = find_by_id(data._id);
    return updated_job.value();
}

int SQliteRepository::delete_job(const std::string& id) {

    SQLite::Statement statement(*db,"DELETE FROM jobs where id = ?");
    statement.bind(1, id);

    return statement.exec();
}

std::optional<JobData> SQliteRepository::find_by_id(const std::string& id){

    SQLite::Statement statement(*db,
            "SELECT id, name, type, status, command, next_run, schedule_payload "
            "FROM jobs WHERE id = ?"
        );
    
    statement.bind(1, id);

    if (statement.executeStep()) {
        JobData job;

        job._id = statement.getColumn(0).getString();
        job._name = statement.getColumn(1).getString();
        job._type = schedule_type_from_string(statement.getColumn(2).getString());
        job._status = job_status_from_string(statement.getColumn(3).getString());
        job._command = statement.getColumn(4).getString();
        job._next_run = std::chrono::sys_seconds(std::chrono::seconds(statement.getColumn(5).getInt64()));
        job._schedule_payload = statement.getColumn(6).getString();

        return job;
    }

    return std::nullopt;
}
    

std::vector<JobData> SQliteRepository::find_all(){
    
    SQLite::Statement statement(*db,
            "SELECT id, name, type, status, command, next_run, schedule_payload FROM jobs");

    std::vector<JobData> jobs;

    while (statement.executeStep()) {
        JobData job;

        job._id = statement.getColumn(0).getString();
        job._name = statement.getColumn(1).getString();
        job._type = schedule_type_from_string(statement.getColumn(2).getString());
        job._status = job_status_from_string(statement.getColumn(3).getString());
        job._command = statement.getColumn(4).getString();
        job._next_run = std::chrono::sys_seconds(std::chrono::seconds(statement.getColumn(5).getInt64()));
        job._schedule_payload = statement.getColumn(6).getString();

        jobs.push_back(job);
    }

    return jobs;
}

std::vector<JobData> SQliteRepository::find_active(){

    SQLite::Statement statement(*db,
            "SELECT id, name, type, status, command, next_run, schedule_payload "
            "FROM jobs WHERE status = 'ACTIVE'"
        );

    std::vector<JobData> jobs;
    
    while (statement.executeStep()) {
        JobData job;

        job._id = statement.getColumn(0).getString();
        job._name = statement.getColumn(1).getString();
        job._type = schedule_type_from_string(statement.getColumn(2).getString());
        job._status = job_status_from_string(statement.getColumn(3).getString());
        job._command = statement.getColumn(4).getString();
        job._next_run = std::chrono::sys_seconds(std::chrono::seconds(statement.getColumn(5).getInt64()));
        job._schedule_payload = statement.getColumn(6).getString();

        jobs.push_back(job);
    }

    return jobs;
}