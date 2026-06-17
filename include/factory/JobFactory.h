#pragma once
#include <functional>
#include <map>
#include <memory>
#include "domain/Job.h"
#include "domain/JobData.h"
#include "domain/ScheduleType.h"

class JobFactory {
public:
    JobFactory();
    std::unique_ptr<Job> create_job(const JobData &data);

private:
    std::map<ScheduleType, std::function<std::unique_ptr<Job>(const JobData&)>> registry;
};