#ifndef HUMBLEENGINE_GAME_H
#define HUMBLEENGINE_GAME_H
#include <expected>
#include "rendering/irenderer.h"

namespace engine {
    class Game {
    public:
        explicit Game(IRenderer& renderer) : renderer(renderer) {}
        virtual ~Game() = default;
        void run();

        // Called once at startup
        virtual void onInitialize() {}

        // Called every frame
        virtual void onUpdate(float deltaTime) = 0;

        // Called every frame after update
        virtual void onRender() = 0;

        // Called once at shutdown
        virtual void onShutdown() {}

        [[nodiscard]] IRenderer& getRenderer() const { return renderer; }

    private:
        IRenderer& renderer;
    };
}

#endif // HUMBLEENGINE_GAME_H
