#pragma once
#include "Job.h"

class IntervalJob : public Job {
public:
    IntervalJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedule_payload);

    explicit IntervalJob(const JobData &data);

    virtual ~IntervalJob() = default;
};
