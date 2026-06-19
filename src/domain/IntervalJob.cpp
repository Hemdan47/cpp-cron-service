#include "domain/IntervalJob.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

IntervalJob::IntervalJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedule_payload)
    : Job(id , name , type , command , status , schedule_payload) {

    json payload = json::parse(get_schedule_payload());
    const std::size_t interval_seconds = payload["interval_seconds"];

    set_interval_seconds(interval_seconds);

    IntervalJob::calculate_next_run();
}



IntervalJob::IntervalJob(const JobData &data) : Job(data) {

    json payload = json::parse(get_schedule_payload());
    const std::size_t interval_seconds = payload["interval_seconds"];

    set_interval_seconds(interval_seconds);
}

std::chrono::sys_seconds IntervalJob::calculate_next_run() {

    const auto now = std::chrono::floor<std::chrono::seconds>(
    std::chrono::system_clock::now());

    if (get_next_run() > now) {
        return get_next_run();
    }

    if (get_next_run() == std::chrono::sys_seconds{}) {
        set_next_run(now + std::chrono::seconds(get_interval_seconds()));
        return get_next_run();
    }

    const auto diff = now - get_next_run();
    const int missed = diff.count() / get_interval_seconds();
    set_next_run(get_next_run() + std::chrono::seconds((missed + 1) * get_interval_seconds()));

    return get_next_run();
}

std::size_t IntervalJob::get_interval_seconds() const{
    return _interval_seconds;
}

void IntervalJob::set_interval_seconds(const std::size_t interval_seconds) {
    _interval_seconds = interval_seconds;
}