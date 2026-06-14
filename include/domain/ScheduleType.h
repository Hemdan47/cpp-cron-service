#pragma once
#include <string>
#include <stdexcept>

enum class ScheduleType {
    CRON,
    INTERVAL,
    ONETIME
};

inline ScheduleType scheduleTypeFromString(const std::string& s) {
    if (s == "CRON")     return ScheduleType::CRON;
    if (s == "INTERVAL") return ScheduleType::INTERVAL;
    if (s == "ONETIME")  return ScheduleType::ONETIME;
    throw std::invalid_argument("Unknown schedule type: " + s);
}

inline std::string scheduleTypeToString(ScheduleType t) {
    switch (t) {
        case ScheduleType::CRON:     return "CRON";
        case ScheduleType::INTERVAL: return "INTERVAL";
        case ScheduleType::ONETIME:  return "ONETIME";
    }
}