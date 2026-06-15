#pragma once
#include "Job.h"

class CronJob : public Job {
public:
    CronJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedule_payload);

    explicit CronJob(const JobData &data);

    virtual ~CronJob() = default;
};