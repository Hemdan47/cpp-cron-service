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

}

int SQliteRepository::delete_job(const std::string& id) {

}

std::optional<JobData> SQliteRepository::find_by_id(const std::string& id) {

}

std::vector<JobData> SQliteRepository::find_all() {

}

std::vector<JobData> SQliteRepository::find_active() {

}
