#ifndef HUMBLEENGINE_ENGINE_CORE_H
#define HUMBLEENGINE_ENGINE_CORE_H

namespace engine {
    class EngineCore final {
    public:
        static EngineCore& getInstance();

        bool initialize();
        void shutdown();

    private:
        bool isInitialized = false;
    };
}

#endif //HUMBLEENGINE_ENGINE_CORE_H