#include "utils/IdGenerator.h"
#include <chrono>
#include <random>

std::string generate_id() {
    using namespace std::chrono;

    const auto now = duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    ).count();

    thread_local std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(10000, 99999);

    return std::to_string(now) + "_" + std::to_string(dist(rng));
}