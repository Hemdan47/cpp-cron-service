#include "domain/OneTimeJob.h"
#include <nlohmann/json.hpp>
#include <chrono>
#include "utils/TimeUtils.h"

using json = nlohmann::json;

OneTimeJob::OneTimeJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedule_payload)
    : Job(id , name , type , command , status , schedule_payload){

    OneTimeJob::calculate_next_run();
}


OneTimeJob::OneTimeJob(const JobData &data) : Job(data) {}

std::chrono::sys_seconds OneTimeJob::calculate_next_run() {

    json payload = json::parse(get_schedule_payload());
    const std::string iso_date = payload["iso_date"];

    const std::chrono::sys_seconds scheduled_time = iso_to_sys_seconds(iso_date);

    set_next_run(scheduled_time);
    return get_next_run();
}