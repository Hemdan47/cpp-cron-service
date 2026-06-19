#pragma once
#include <set>
#include "Job.h"

class CronJob : public Job {
public:
    CronJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedule_payload);
    explicit CronJob(const JobData &data);

    virtual std::chrono::sys_seconds calculate_next_run() override;

    virtual ~CronJob() = default;

private:
    std::set<int> _minutes;
    std::set<int> _hours;
    std::set<int> _days_of_month;
    std::set<int> _months;
    std::set<int> _days_of_week;

    void _parse_expression(const std::string& expression);
    static std::set<int> _parse_field(const std::string& field, int min, int max);
};