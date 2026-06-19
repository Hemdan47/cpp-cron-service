#pragma once
#include <memory>
#include <mutex>
#include "IJobRepository.h"
#include "SQLiteCpp/Database.h"

class SQliteRepository : public IJobRepository{
public:

    SQliteRepository();


    virtual JobData save_job(const JobData& data) override;
    virtual JobData update_job(const JobData& data) override;
    virtual int delete_job(const std::string& id) override;
    virtual std::optional<JobData> find_by_id(const std::string& id) override;
    virtual std::vector<JobData> find_all() override;
    virtual std::vector<JobData> find_active() override;

    virtual ~SQliteRepository() = default;


private:
    void initialize_schema() const;

    std::unique_ptr<SQLite::Database> db;
    mutable std::recursive_mutex _mutex;
};
