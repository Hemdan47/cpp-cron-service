#pragma once
#include <string>
#include <optional>
#include <vector>
#include "domain/JobData.h"


class IJobRepository {
public:

    virtual JobData save_job(const JobData& data) = 0;
    virtual JobData update_job(const JobData& data) = 0;
    virtual int delete_job(const std::string& id) = 0;
    virtual std::optional<JobData> find_by_id(const std::string& id) = 0;
    virtual std::vector<JobData> find_all() = 0;
    virtual std::vector<JobData> find_active() = 0;

    virtual ~IJobRepository() = default;

};
