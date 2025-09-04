#ifndef BASELAYER_LOGGER_IMPL_H
#define BASELAYER_LOGGER_IMPL_H
#include <fstream>
#include <sstream>
#include "ilogger.h"

namespace engine {
    class Logger : public ILogger {
    public:
        Logger();
        ~Logger() override;

        void debug(const std::string& message) override;
        void info(const std::string& message) override;
        void warning(const std::string& message) override;
        void error(const std::string& message) override;
        void critical(const std::string& message) override;

    private:
        static std::string getCurrentTimestamp();
        static std::string levelToString(LogLevel level);

        std::ofstream logFile;
        bool consoleOutput;
        bool fileOutput;
        std::string logFilename;

        void setupLogFile();
        void setLogFile(const std::string& filename);
        void writeLog(LogLevel level, const std::string& message);
    };
}

#endif // BASELAYER_LOGGER_IMPL_H
