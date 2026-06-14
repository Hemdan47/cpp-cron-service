#pragma once
#include <string>
#include <optional>
#include <vector>


class IJobRepository {
public:

    virtual void saveJob(const JobDTO& data) = 0;
    virtual void updateJob(const JobDTO& data) = 0;
    virtual void deleteJob(const std::string& id) = 0;
    virtual std::optional<JobDTO> findById(const std::string& id) = 0;
    virtual std::vector<JobDTO> findAll() = 0;
    virtual std::vector<JobDTO> findActive() = 0;

    virtual ~IJobRepository() = default;

};