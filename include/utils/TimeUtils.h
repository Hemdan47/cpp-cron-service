#pragma once
#include <chrono>
#include <string>

std::chrono::sys_seconds iso_to_sys_seconds(const std::string& iso_date);