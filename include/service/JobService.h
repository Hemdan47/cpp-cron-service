#pragma once
#include <memory>

#include "factory/JobFactory.h"
#include "repository/IJobRepository.h"
#include "scheduler/SchedulerDaemon.h"

class JobService {

public:

    JobService(
        std::shared_ptr<IJobRepository> repo,
        std::shared_ptr<JobFactory> factory,
        std::shared_ptr<SchedulerDaemon> daemon
    );

    JobData create_job(JobData data);
    JobData pause_job(const std::string& id);
    JobData resume_job(const std::string& id);
    void delete_job(const std::string& id);
    std::vector<JobData> get_all_jobs();
    JobData get_job_by_id(const std::string& id);


private:
    std::shared_ptr<IJobRepository> _repo;
    std::shared_ptr<JobFactory> _factory;
    std::shared_ptr<SchedulerDaemon> _daemon;
};
