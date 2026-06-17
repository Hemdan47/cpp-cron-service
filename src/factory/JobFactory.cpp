#include "factory/JobFactory.h"
#include <stdexcept>

Job

std::unique_ptr<Job> JobFactory::create_job(const JobData &data) {

    switch (data._type) {

        case ScheduleType::CRON:
            return std::make_unique<CronJob>(data);

        case ScheduleType::INTERVAL:
            return std::make_unique<IntervalJob>(data);

        case ScheduleType::ONETIME:
            return std::make_unique<OneTimeJob>(data);

        default:
            throw std::invalid_argument("Unknown schedule type in JobFactory");
    }
}