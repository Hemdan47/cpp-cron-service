#include "domain/OneTimeJob.h"
#include <nlohmann/json.hpp>
#include <sstream>

using json = nlohmann::json;

OneTimeJob::OneTimeJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedule_payload)
    : Job(id , name , type , command , status , schedule_payload)

    // OneTimeJob::calculate_next_run();


OneTimeJob::OneTimeJob(const JobData &data) : Job(data) {}

std::chrono::sys_seconds OneTimeJob::calculate_next_run(std::chrono::sys_seconds currentTime) {
    json payload = json::parse(this->schedule_payload);

    std::string iso_date = payload["iso_date"];

    std::tm tm = {};
    std::istringstream ss(iso_date);

    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

    std::time_t time = std::mktime(&tm);

    return std::chrono::sys_seconds(std::chrono::seconds(time));
}