#include "factory/JobFactory.h"
#include <stdexcept>
#include "domain/CronJob.h"
#include "domain/IntervalJob.h"
#include "domain/OneTimeJob.h"

JobFactory::JobFactory() {

    _registry[ScheduleType::CRON]     = [](const JobData& d) { return std::make_unique<CronJob>(d);};
    _registry[ScheduleType::INTERVAL] = [](const JobData& d) { return std::make_unique<IntervalJob>(d);};
    _registry[ScheduleType::ONETIME]  = [](const JobData& d) { return std::make_unique<OneTimeJob>(d);};

}


std::unique_ptr<Job> JobFactory::create_job(const JobData &data) const{

    const auto it = _registry.find(data._type);
    if (it != _registry.end()) {
        return it->second(data);
    }

    throw std::invalid_argument("Unknown schedule type");

}