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

void SQliteRepository::delete_job(const std::string& id) {

}

std::optional<JobData> SQliteRepository::find_by_id(const std::string& id) {

}

std::vector<JobData> SQliteRepository::find_all() {

}

std::vector<JobData> SQliteRepository::find_active() {

}
