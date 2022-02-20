#pragma once

#include <chrono>
#include <string>
#include <map>
#include "logger.hpp"

namespace damascus {

class Timer {
private:
    std::string name = "";
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    Logger& logger;
    
public:
    // TODO: Add user selectable units
    Timer(const std::string name, Logger& logger);
    ~Timer();
};

}
