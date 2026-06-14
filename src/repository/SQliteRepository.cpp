#include <iostream>
#include <repository/SQliteRepository.h>
#include "SQLiteCpp/Database.h"

SQliteRepository::SQliteRepository() {
    try {
        SQLite::Database    db("job.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
        db.exec(R"(
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
    catch (std::exception& e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}


