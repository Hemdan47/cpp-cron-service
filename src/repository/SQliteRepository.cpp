#include <iostream>
#include <optional>
#include <repository/SQliteRepository.h>
#include "SQLiteCpp/Database.h"

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

void SQliteRepository::save_job(const JobData& data) {

}

void SQliteRepository::update_job(const JobData& data) {

}

void SQliteRepository::delete_job(const JobData& data) {

}

std::optional<JobData> SqliteRepository::find_by_id(const std::string& id)
{
    try {
        SQLite::Statement query(*db,
            "SELECT id, name, type, status, command, next_run, schedule_payload "
            "FROM jobs WHERE id = ?"
        );

        query.bind(1, id);

        if (query.executeStep()) {
            JobData job;

            job.id = query.getColumn(0).getString();
            job.name = query.getColumn(1).getString();
            job.type = query.getColumn(2).getString();
            job.status = query.getColumn(3).getString();
            job.command = query.getColumn(4).getString();
            job.next_run = query.getColumn(5).getInt64();
            job.schedule_payload = query.getColumn(6).getString();

            return job;
        }

        return std::nullopt;
    }
    catch (const std::exception& e) {
        std::cout << "find_by_id error: " << e.what() << std::endl;
        return std::nullopt;
    }
}    


std::vector<JobData> SQliteRepository::find_all()
{
    std::vector<JobData> jobs;

    try {
        SQLite::Statement query(*db,
            "SELECT id, name, type, status, command, next_run, schedule_payload FROM jobs"
        );

        while (query.executeStep()) {
            JobData job;

            job.id = query.getColumn(0).getString();
            job.name = query.getColumn(1).getString();
            job.type = query.getColumn(2).getString();
            job.status = query.getColumn(3).getString();
            job.command = query.getColumn(4).getString();
            job.next_run = query.getColumn(5).getInt64();
            job.schedule_payload = query.getColumn(6).getString();

            jobs.push_back(job);
        }
    }
    catch (std::exception& e) {
        std::cout << "find_all error: " << e.what() << std::endl;
    }

    return jobs;
}

std::vector<JobData> SQliteRepository::find_active()
{
    std::vector<JobData> jobs;

    try {
        SQLite::Statement query(*db,
            "SELECT id, name, type, status, command, next_run, schedule_payload "
            "FROM jobs WHERE status = 'ACTIVE'"
        );

        while (query.executeStep()) {
            JobData job;

            job.id = query.getColumn(0).getString();
            job.name = query.getColumn(1).getString();
            job.type = query.getColumn(2).getString();
            job.status = query.getColumn(3).getString();
            job.command = query.getColumn(4).getString();
            job.next_run = query.getColumn(5).getInt64();
            job.schedule_payload = query.getColumn(6).getString();

            jobs.push_back(job);
        }
    }
    catch (std::exception& e) {
        std::cout << "find_active error: " << e.what() << std::endl;
    }

    return jobs;
}