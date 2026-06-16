#pragma once
#include "Job.h"

class OneTimeJob : public Job {
public:
    OneTimeJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedule_payload);
    explicit OneTimeJob(const JobData &data);

    virtual std::chrono::sys_seconds calculate_next_run() override;

    virtual ~OneTimeJob() = default;
};
