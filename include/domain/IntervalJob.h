#pragma once
#include "Job.h"

class IntervalJob : public Job {
public:
    IntervalJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedulePayload)
        : Job(id , name , type , command , status , schedulePayload){}

    explicit IntervalJob(const JobData &data) : Job(data){}

    virtual ~IntervalJob() = default;
};
