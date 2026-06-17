#pragma once
#include <stdexcept>

class JobNotFoundException : public std::runtime_error {
public:
    explicit JobNotFoundException(const std::string& message) : std::runtime_error(message) {}

};
