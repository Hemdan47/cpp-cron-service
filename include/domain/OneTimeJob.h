#pragma once
#include "Job.h"

class OneTimeJob : public Job {
public:
    OneTimeJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedulePayload)
        : Job(id , name , type , command , status , schedulePayload){}

    explicit OneTimeJob(const JobData &data) : Job(data){}

    virtual ~OneTimeJob() = default;
};
