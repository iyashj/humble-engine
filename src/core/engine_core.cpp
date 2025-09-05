#include "engine_core.h"

namespace engine {
    EngineCore& EngineCore::getInstance() {
        static EngineCore instance;
        return instance;
    }

    bool EngineCore::initialize() {
        if (isInitialized) {
            return true;
        }

        // initialize logging
        // log that engine has been initialized

        isInitialized = true;
        return true;
    }

    void EngineCore::shutdown() {
        if (!isInitialized)
        {
            return;
        }

        // log that engine core is being shutdown

        isInitialized = false;
    }
}
