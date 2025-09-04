#include "logger.h"
#include <iostream>
#include <chrono>

namespace engine {

    Logger::Logger() : consoleOutput(true), fileOutput(true) {
        setupLogFile();
    }
    Logger::~Logger() {
        if (logFile.is_open()) {
            logFile << "=== Log Ended ===" << std::endl;
            logFile.close();
        }
    }

    Logger& Logger::getInstance() {
        static Logger instance;
        return instance;
    }

    void Logger::debug(const std::string& message) { writeLog(LogLevel::Debug, message); }
    void Logger::info(const std::string& message) { writeLog(LogLevel::Info, message); }
    void Logger::warning(const std::string& message) { writeLog(LogLevel::Warning, message); }
    void Logger::error(const std::string& message) { writeLog(LogLevel::Error, message); }
    void Logger::critical(const std::string& message) { writeLog(LogLevel::Critical, message); }

    std::string Logger::getCurrentTimestamp() {
        const auto now = std::chrono::system_clock::now();
        const auto time_t = std::chrono::system_clock::to_time_t(now);
        const auto tm = *std::localtime(&time_t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::string Logger::levelToString(const LogLevel level) {
        switch (level) {
            case LogLevel::Debug: return "DEBUG";
            case LogLevel::Info: return "INFO";
            case LogLevel::Warning: return "WARNING";
            case LogLevel::Error: return "ERROR";
            case LogLevel::Critical: return "CRITICAL";
        }
        return "Unknown";
    }

    void Logger::setupLogFile() {
        std::filesystem::create_directories("logs");
        const auto now = std::chrono::system_clock::now();
        const auto time_t = std::chrono::system_clock::to_time_t(now);
        const auto tm = *std::localtime(&time_t);
        std::ostringstream oss;
        oss << "logs/baselayer_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".log";
        logFilename = oss.str();
        if (fileOutput) {
            logFile.open(logFilename, std::ios::app);
            if (logFile.is_open()) {
                logFile << "=== BaseLayer Engine Log Started ===" << std::endl;
            }
        }
    }

    void Logger::setLogFile(const std::string &filename) {
        if (logFile.is_open()) logFile.close();
        logFilename = filename;
        if (fileOutput) logFile.open(logFilename, std::ios::app);
    }

    void Logger::writeLog(const LogLevel level, const std::string& message) {
        const std::string timestamp = getCurrentTimestamp();
        const std::string level_str = levelToString(level);
        const std::string log_line = "[" + timestamp + "] [" + level_str + "] " + message;
        if (consoleOutput) {
            if (level >= LogLevel::Error) std::cerr << log_line << std::endl;
            else std::cout << log_line << std::endl;
        }
        if (fileOutput) {
            if (!logFile.is_open() && !logFilename.empty()) {
                logFile.open(logFilename, std::ios::app);
            }
            if (logFile.is_open()) {
                logFile << log_line << std::endl;
                logFile.flush();
            }
        }
    }

}
