// CronJob.cpp
#include "domain/CronJob.h"
#include <nlohmann/json.hpp>
#include <sstream>
#include <ctime>

#include "exceptions/InvalidScheduleException.h"
#include "utils/TimeUtils.h"

using json = nlohmann::json;

CronJob::CronJob(std::string id , std::string name , ScheduleType type , std::string command , JobStatus status , std::string schedule_payload)
    : Job(id , name , type , command , status , schedule_payload) {

    json payload = json::parse(get_schedule_payload());
    const std::string expression = payload["expression"];

    _parse_expression(expression);

    CronJob::calculate_next_run();
}

CronJob::CronJob(const JobData &data) : Job(data) {

    json payload = json::parse(get_schedule_payload());
    const std::string expression = payload["expression"];

    _parse_expression(expression);
}

void CronJob::_parse_expression(const std::string& expression) {
    std::istringstream ss(expression);
    std::string minute, hour, day_of_month, month, day_of_week;
    ss >> minute >> hour >> day_of_month >> month >> day_of_week;

    if (minute.empty() || hour.empty() || day_of_month.empty() || month.empty() || day_of_week.empty()) {
        throw InvalidScheduleException("Invalid cron expression: " + expression);
    }

    _minutes       = _parse_field(minute, 0, 59);
    _hours         = _parse_field(hour, 0, 23);
    _days_of_month = _parse_field(day_of_month, 1, 31);
    _months        = _parse_field(month, 1, 12);
    _days_of_week  = _parse_field(day_of_week, 0,  6);
}

std::set<int> CronJob::_parse_field(const std::string& field, int min, int max) {
    std::set<int> values;

    if (field == "*") {
        for (int i = min; i <= max; i++) values.insert(i);
        return values;
    }

    const auto dash = field.find('-');
    if (dash != std::string::npos) {
        const int start = std::stoi(field.substr(0, dash));
        const int end   = std::stoi(field.substr(dash + 1));

        if (start < min || end > max || start > end) {
            throw InvalidScheduleException("Invalid cron range: " + field);
        }

        for (int i = start; i <= end; i++) values.insert(i);
        return values;
    }

    const int value = std::stoi(field);
    if (value < min || value > max) {
        throw InvalidScheduleException("Cron value out of range: " + field);
    }
    values.insert(value);

    return values;
}

std::chrono::sys_seconds CronJob::calculate_next_run() {

    const auto now = std::chrono::floor<std::chrono::seconds>(
        std::chrono::system_clock::now());

    if (get_next_run() > now) {
        return get_next_run();
    }

    auto candidate = std::chrono::floor<std::chrono::minutes>(now) + std::chrono::minutes(1);
    const auto limit = candidate + std::chrono::hours(24 * 366);

    while (candidate < limit) {
        auto [month, day, hour, minute, weekday] = sys_seconds_to_calendar(candidate);

        if (_months.count(month) &&
            _days_of_month.count(day) &&
            _days_of_week.count(weekday) &&
            _hours.count(hour) &&
            _minutes.count(minute))
        {
            set_next_run(candidate);
            return get_next_run();
        }

        candidate += std::chrono::minutes(1);
    }

    throw InvalidScheduleException("Could not resolve next run time for cron expression");
}