#ifndef RENDERER_H
#define RENDERER_H
#include <string>
#include "raylib.h"
#include "irenderer.h"

namespace engine {
    class Renderer : public IRenderer {
    public:
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        ~Renderer() override = default;

        void initialise(int width, int height, const std::string& title) override;
        void shutdown() override;

        void beginFrame() override;
        void endFrame() override;
        [[nodiscard]] bool shouldClose() const override;

        void setRenderTarget(RenderTexture2D& renderTarget) override;
        void clearRenderTarget() override;

        void drawCircle(const Vector2& position, float radius, const Color& color) override;
        void drawTexture(const Texture2D& texture,
            const Rectangle& source,
            const Rectangle& dest,
            const Vector2& origin,
            float rotation,
            const Color& tint) override;
        void drawText(const std::string& text,
            const Vector2& position,
            int fontSize,
            const Color& color) override;

        [[nodiscard]] int getWindowWidth() const override;
        [[nodiscard]] int getWindowHeight() const override;
        [[nodiscard]] RenderTexture2D& getRenderTexture() override;
        [[nodiscard]] const Vector2 getScreenCenter() const override;

    private:
        int windowWidth;
        int windowHeight;
        RenderTexture2D* renderTexture;
        Vector2 screenCenter;
        bool windowShouldClose;
    };
}

#endif// RENDERER_H
