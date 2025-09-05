#include "game/game.h"

namespace engine {

    void Game::run() {
        onInitialize();

        while (!renderer.shouldClose()) {
            renderer.beginFrame();
            onUpdate(GetFrameTime()); // Replace with actual delta time if available
            onRender();
            renderer.endFrame();
        }

        onShutdown();
        renderer.shutdown();
    }

}
