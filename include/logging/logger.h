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
        static void logCallback(int logLevel, const char *text, va_list args);
        static const char* getLevelString(int level);
    };


// Variadic macros for printf-style logging
#define LOG_TRACE_MSG(...) TraceLog(LOG_TRACE, __VA_ARGS__)
#define LOG_DEBUG_MSG(...) TraceLog(LOG_DEBUG, __VA_ARGS__)
#define LOG_INFO_MSG(...)  TraceLog(LOG_INFO,  __VA_ARGS__)
#define LOG_WARN_MSG(...)  TraceLog(LOG_WARNING, __VA_ARGS__)
#define LOG_ERROR_MSG(...) TraceLog(LOG_ERROR, __VA_ARGS__)
#define LOG_FATAL_MSG(...) TraceLog(LOG_FATAL, __VA_ARGS__)

}

#endif //HUMBLEENGINE_HUMBLE_LOGGER_H