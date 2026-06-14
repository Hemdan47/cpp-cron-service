#pragma once
#include "IJobRepository.h"

class SQliteRepository : public IJobRepository{
public:

    SQliteRepository();

    virtual void saveJob(const JobDTO& data) override;
    virtual void updateJob(const JobDTO& data) override;
    virtual void deleteJob(const std::string& id) override;
    virtual std::optional<JobDTO> findById(const std::string& id) override;
    virtual std::vector<JobDTO> findAll() override;
    virtual std::vector<JobDTO> findActive() override;

    virtual ~SQliteRepository();
};
