#pragma once
#include <chrono>
#include <string>
#include "domain/ScheduleType.h"
#include "domain/JobStatus.h"

class JobData {
public:
    std::string  id;
    std::string  name;
    ScheduleType type;
    std::string  command;
    JobStatus    status;
    std::chrono::sys_seconds _next_run;
    std::string  schedulePayload;
};