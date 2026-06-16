#pragma once

#include <memory>
#include "domain/Job.h"
#include "domain/JobData.h"
#include "domain/ScheduleType.h"

#include "domain/CronJob.h"
#include "domain/IntervalJob.h"
#include "domain/OneTimeJob.h"

class JobFactory {
public:
    std::unique_ptr<Job> create_job(const JobData &data);
};