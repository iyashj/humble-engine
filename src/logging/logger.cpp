#include "logging/logger.h"
#include <fstream>
#include <iostream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <format>
#include <fmt/core.h>

namespace engine {

    void Logger::initialize(const std::string &logFile, int level) {
        std::string logFileName = logFile;
        if (!logFile.empty()) {
            // Add date-time tag to filename before extension
            const auto now = std::chrono::system_clock::now();
            auto now_time_t = std::chrono::system_clock::to_time_t(now);
            std::tm tm_buf{};
#if defined(_WIN32)
            localtime_s(&tm_buf, &now_time_t);
#else
            localtime_r(&now_time_t, &tm_buf);
#endif
            std::ostringstream oss;
            oss << std::put_time(&tm_buf, "%Y%m%d_%H%M%S");
            // Insert timestamp before file extension
            if (const size_t dot = logFile.find_last_of('.'); dot != std::string::npos) {
                logFileName = logFile.substr(0, dot) + "_" + oss.str() + logFile.substr(dot);
            } else {
                logFileName = logFile + "_" + oss.str();
            }
            logFileStream.open(logFileName, std::ios::app);
        }
        SetTraceLogCallback(logCallback);
        SetTraceLogLevel(level);
    }
    void Logger::shutdown() {
        if (logFileStream.is_open()) {
            logFileStream.close();
        }
    }

    std::ofstream Logger::logFileStream;
    std::mutex Logger::logMutex;

    void Logger::logCallback(int logLevel, const char *text, va_list args) {
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), text, args);
        std::string safeMsg(buffer, strnlen(buffer, sizeof(buffer)));

        // Get timestamp
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm tm_buf{};
#if defined(_WIN32)
        localtime_s(&tm_buf, &now_time_t);
#else
        localtime_r(&now_time_t, &tm_buf);
#endif
        std::ostringstream timestamp;
        timestamp << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");

    const std::string formatted = fmt::format("[{}] [{}] {}", timestamp.str(), getLevelString(logLevel), safeMsg);
        // Thread-safe output
        const std::lock_guard<std::mutex> lock(logMutex);
        std::cout << formatted << '\n';
        if (logFileStream.is_open()) {
            logFileStream << formatted << '\n';
            logFileStream.flush();
        }
    }

    const char * Logger::getLevelString(int level) {
        switch (level) {
            case LOG_TRACE: return "TRACE";
            case LOG_DEBUG: return "DEBUG";
            case LOG_INFO: return "INFO";
            case LOG_WARNING: return "WARNING";
            case LOG_ERROR: return "ERROR";
            case LOG_FATAL: return "FATAL";
            default: return "UNKNOWN";
        }
    }
}
