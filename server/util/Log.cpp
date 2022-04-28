#include "util/Log.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>

#include <errno.h>
#include <string.h>

#define WRITE(level) if (level >= Log::minLevel) writeLog(LogMessage{level, sessionId, message})

static const char* LogLevelNames[] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

mutex Log::_mutex;
LogLevel Log::minLevel;

void Log::init(LogLevel minLevel)
{
    Log::minLevel = minLevel;
}

void Log::changeMinLevel(LogLevel minLevel)
{
    Log::minLevel = minLevel;
}

void Log::debug(std::string message, UUID sessionId)
{
    WRITE(DEBUG);
}

void Log::info(std::string message, UUID sessionId)
{
    WRITE(INFO);
}

void Log::warn(std::string message, UUID sessionId)
{
    WRITE(WARN);
}

void Log::error(std::string message, UUID sessionId)
{
    WRITE(ERROR);
}

void Log::errorNo(std::string message, UUID sessionId)
{
    message += " Additional error info: " + string(strerror(errno));
    WRITE(ERROR);
}

void Log::fatal(std::string message, UUID sessionId)
{
    WRITE(FATAL);
}

void Log::writeLog(LogMessage message)
{
    unique_lock<mutex> lock(_mutex);

    char separator = '\t';

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout <<  put_time(localtime(&now), "%F-%H-%M-%S");
    std::cout << separator;
    std::cout << LogLevelNames[message.level];
    std::cout << separator;
    if (message.sessionId != UUID::empty)
        std::cout << message.sessionId.toStr();
    std::cout << separator;
    std::cout << message.message;

    std::cout << std::endl;
}