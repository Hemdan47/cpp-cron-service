#pragma once
#include "Job.h"

class IntervalJob : public Job {
public:
    IntervalJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedule_payload);
    explicit IntervalJob(const JobData &data);

    virtual std::chrono::sys_seconds calculate_next_run() override;

    std::size_t get_interval_seconds() const;
    void set_interval_seconds(std::size_t interval_seconds);

    virtual ~IntervalJob() = default;



private:
    std::size_t _interval_seconds;
};
