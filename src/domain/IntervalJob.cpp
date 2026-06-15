#include "domain/IntervalJob.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

IntervalJob::IntervalJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedule_payload)
    : Job(id , name , type , command , status , schedule_payload)

    // IntervalJob::calculate_next_run();


IntervalJob::IntervalJob(const JobData &data) : Job(data) {}

std::chrono::sys_seconds IntervalJob::calculate_next_run(std::chrono::sys_seconds currentTime) {
    json payload = json::parse(this->schedule_payload);

    int interval_seconds = payload["interval_seconds"];

    return currentTime + std::chrono::seconds(interval_seconds);
}