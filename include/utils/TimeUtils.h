#pragma once
#include <chrono>
#include <string>

std::tuple<unsigned, unsigned, int, int, int> sys_seconds_to_calendar(std::chrono::sys_seconds tp);
std::chrono::sys_seconds iso_to_sys_seconds(const std::string& iso_date);