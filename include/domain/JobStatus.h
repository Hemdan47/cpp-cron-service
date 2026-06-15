#pragma once
#include <string>
#include <stdexcept>

enum class JobStatus {
    ACTIVE,
    PAUSED,
    RUNNING,
    COMPLETED,
    FAILED
};


inline JobStatus job_status_from_string(const std::string& s) {
    if (s == "ACTIVE")    return JobStatus::ACTIVE;
    if (s == "PAUSED")    return JobStatus::PAUSED;
    if (s == "RUNNING")   return JobStatus::RUNNING;
    if (s == "COMPLETED") return JobStatus::COMPLETED;
    if (s == "FAILED")    return JobStatus::FAILED;
    throw std::invalid_argument("Unknown job status: " + s);
}

inline std::string job_status_to_string(JobStatus s) {
    switch (s) {
        case JobStatus::ACTIVE:    return "ACTIVE";
        case JobStatus::PAUSED:    return "PAUSED";
        case JobStatus::RUNNING:   return "RUNNING";
        case JobStatus::COMPLETED: return "COMPLETED";
        case JobStatus::FAILED:    return "FAILED";
    }
}