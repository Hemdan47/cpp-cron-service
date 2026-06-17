#pragma once
#include <stdexcept>

class InvalidScheduleException : public std::runtime_error {
public:
    explicit InvalidScheduleException(const std::string& message) : std::runtime_error(message) {}

};
