#ifndef HUMBLEENGINE_HUMBLE_LOGGER_H
#define HUMBLEENGINE_HUMBLE_LOGGER_H

#include <string>
#include <raylib.h>
#include <mutex>

namespace engine {

    class Logger {
    public:
        static void initialize(const std::string &logFile = "", int level = LOG_ALL);
        static void shutdown();

    private:
        static std::ofstream logFileStream;
        static std::mutex logMutex;
        static void logCallback(int logLevel, const char *text, const va_list args);
        static const char* getLevelString(int level);
    };


// Variadic macros for printf-style logging
#define LOG_TRACE_MSG(fmt, ...) TraceLog(LOG_TRACE, fmt, ##__VA_ARGS__)
#define LOG_DEBUG_MSG(fmt, ...) TraceLog(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define LOG_INFO_MSG(fmt, ...)  TraceLog(LOG_INFO,  fmt, ##__VA_ARGS__)
#define LOG_WARN_MSG(fmt, ...)  TraceLog(LOG_WARNING, fmt, ##__VA_ARGS__)
#define LOG_ERROR_MSG(fmt, ...) TraceLog(LOG_ERROR, fmt, ##__VA_ARGS__)
#define LOG_FATAL_MSG(fmt, ...) TraceLog(LOG_FATAL, fmt, ##__VA_ARGS__)

}

#endif //HUMBLEENGINE_HUMBLE_LOGGER_H