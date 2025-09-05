#include "engine_core.h"

#include "logging/logger.h"

namespace engine {
    EngineCore& EngineCore::getInstance() {
        static EngineCore instance;
        return instance;
    }

    bool EngineCore::initialize() {
        if (isInitialized) {
            return true;
        }

        Logger::initialize("HumbleEngine.log");
        LOG_INFO_MSG("HumbleEngine initialized");

        isInitialized = true;
        return true;
    }

    void EngineCore::shutdown() {
        if (!isInitialized)
        {
            return;
        }

        LOG_INFO_MSG("HumbleEngine shutdown");

        isInitialized = false;
    }
}
