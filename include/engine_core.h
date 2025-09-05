#ifndef HUMBLEENGINE_ENGINE_CORE_H
#define HUMBLEENGINE_ENGINE_CORE_H

namespace engine {
    class EngineCore final {
    public:
        static EngineCore& getInstance();

        bool initialize();
        void shutdown();

    private:
        EngineCore() = default;
        ~EngineCore() = default;

        EngineCore(const EngineCore&) = delete;
        EngineCore& operator=(const EngineCore&) = delete;
        EngineCore(EngineCore&&) = delete;
        EngineCore& operator=(EngineCore&&) = delete;
        bool isInitialized = false;
    };
}

#endif //HUMBLEENGINE_ENGINE_CORE_H