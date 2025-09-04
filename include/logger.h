#ifndef BASELAYER_LOGGER_IMPL_H
#define BASELAYER_LOGGER_IMPL_H
#include <fstream>
#include <sstream>

namespace engine {

    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    class Logger {
    public:
        static Logger& getInstance();
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
        Logger(Logger&&) = delete;
        Logger& operator=(Logger&&) = delete;
        ~Logger();

        void debug(const std::string& message);
        void info(const std::string& message);
        void warning(const std::string& message);
        void error(const std::string& message);
        void critical(const std::string& message);

        void setLogFile(const std::string& filename);

    private:
        Logger();
        static std::string getCurrentTimestamp();
        static std::string levelToString(LogLevel level);

        std::ofstream logFile;
        bool consoleOutput;
        bool fileOutput;
        std::string logFilename;

        void setupLogFile();
        void writeLog(LogLevel level, const std::string& message);
    };
}

#endif // BASELAYER_LOGGER_IMPL_H
