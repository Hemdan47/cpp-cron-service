#pragma once
#include "Job.h"

class OneTimeJob : public Job {
public:
    OneTimeJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedulePayload);

    explicit OneTimeJob(const JobData &data);

    virtual ~OneTimeJob() = default;
};
