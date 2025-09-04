#ifndef ILOGGER_H
#define ILOGGER_H
#include <string>

namespace engine {

    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    class ILogger {
    public:
        virtual ~ILogger() = default;

        virtual void debug(const std::string& message) = 0;
        virtual void info(const std::string& message) = 0;
        virtual void warning(const std::string& message) = 0;
        virtual void error(const std::string& message) = 0;
        virtual void critical(const std::string& message) = 0;
    };
}
#endif // ILOGGER_H
