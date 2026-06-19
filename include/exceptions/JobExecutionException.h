#pragma once
#include <stdexcept>

class JobExecutionException : public std::runtime_error {
public:
    explicit JobExecutionException(const std::string& id, const std::string& msg)
        : std::runtime_error("Job " + id + " failed: " + msg) {}
};
