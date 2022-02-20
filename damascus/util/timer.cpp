//
//  timer.cpp
//  damascus
//
//  Created by Kavi Dey on 2/20/22.
//

#include <iostream>
#include <stdio.h>
#include <chrono>
#include "timer.hpp"
#include "logger.hpp"

damascus::Timer::Timer(const std::string name): name(name) {
    start_time = std::chrono::high_resolution_clock::now();
}

damascus::Timer::~Timer() {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    
    DAMASCUS_INFO(name << " took " << duration.count() << " us")
}
