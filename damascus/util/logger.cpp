#include <iostream>
#include "logger.hpp"

damascus::Logger::Logger() : m_LogLevel(LevelInfo) {
}

damascus::Logger::Logger(Level level) : m_LogLevel(level) {
}

void damascus::Logger::SetLevel(Level level) {
    m_LogLevel = level;
}

void damascus::Logger::Info(const char* message) const {
    if (m_LogLevel >= LevelInfo)
        std::cout << "[INFO]: " <<  message << std::endl;
}

void damascus::Logger::Warn(const char* message) const {
    if (m_LogLevel >= LevelWarning)
        std::cout << "[WARNING]: " <<  message << std::endl;
}

void damascus::Logger::Error(const char* message) const {
    if (m_LogLevel >= LevelError)
        std::cout << "[ERROR]: " <<  message << std::endl;
}

damascus::Logger::~Logger() {
    
}
