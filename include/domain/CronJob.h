#pragma once
#include "Job.h"

class CronJob : public Job {
public:
    CronJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedulePayload)
        : Job(id , name , type , command , status , schedulePayload){}

    explicit CronJob(const JobData &data) : Job(data){}

    virtual ~CronJob() = default;
};