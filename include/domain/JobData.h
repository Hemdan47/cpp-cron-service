#pragma once
#include <chrono>
#include <string>
#include "domain/ScheduleType.h"
#include "domain/JobStatus.h"

class JobData {
public:
    std::string  _id;
    std::string  _name;
    ScheduleType _type;
    std::string  _command;
    JobStatus    _status;
    std::chrono::sys_seconds _next_run;
    std::string  _schedule_payload;
};