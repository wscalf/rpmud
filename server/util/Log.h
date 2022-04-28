#pragma once

#include <string>
#include <util/BlockingQueue.h>
#include <util/UUID.h>
#include <mutex>

namespace std
{
    class thread;
}

enum LogLevel
{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

struct LogMessage
{
    LogLevel level;
    UUID sessionId;
    std::string message;
};

class Log
{
    public: //To access to the current sessionId when available
        static void debug(std::string message, UUID sessionId = UUID::empty);
        static void info(std::string message, UUID sessionId = UUID::empty);
        static void warn(std::string message, UUID sessionId = UUID::empty);
        static void error(std::string message, UUID sessionId = UUID::empty);
        static void errorNo(std::string message, UUID sessionId = UUID::empty);
        static void fatal(std::string message, UUID sessionId = UUID::empty);
        static void init(LogLevel minLevel);
        static void changeMinLevel(LogLevel level);
    private:
        static LogLevel minLevel;
        static mutex _mutex;
        static void writeLog(LogMessage message);
};