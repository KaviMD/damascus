#pragma once

#include <stdio.h>

namespace damascus {

class Logger {
public:
    enum Level {
        LevelError = 0, LevelWarning, LevelInfo, LevelDebug
    };
private:
    Level m_LogLevel;
public:
    Logger();
    Logger(Level level);
    
    void SetLevel(Level level);
    
    void Debug(const char* message) const;
    void Info(const char* message) const;
    void Warn(const char* message) const;
    void Error(const char* message) const;
    
    ~Logger();
};
}
