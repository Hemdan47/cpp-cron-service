#pragma once
#include <string>
#include <stdexcept>

enum class ScheduleType {
    CRON,
    INTERVAL,
    ONETIME
};

inline ScheduleType schedule_type_from_string(const std::string& s) {
    if (s == "CRON")     return ScheduleType::CRON;
    if (s == "INTERVAL") return ScheduleType::INTERVAL;
    if (s == "ONETIME")  return ScheduleType::ONETIME;
    throw std::invalid_argument("Unknown schedule type: " + s);
}

inline std::string schedule_type_to_string(ScheduleType t) {
    switch (t) {
        case ScheduleType::CRON:     return "CRON";
        case ScheduleType::INTERVAL: return "INTERVAL";
        case ScheduleType::ONETIME:  return "ONETIME";
        default: throw std::invalid_argument("Unknown schedule type:");
    }
}