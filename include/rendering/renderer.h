#ifndef RENDERER_H
#define RENDERER_H
#include <expected>
#include <memory>
#include <variant>
#include <string>
#include "raylib.h"
#include <vector>
#include "../rendering/irenderer.h"

namespace engine {

    struct CircleParams {
        Vector2 position;
        float radius;
        Color color;
    };
    struct TextureParams {
        const Texture2D* texture;
        Rectangle source;
        Rectangle dest;
        Vector2 origin;
        float rotation;
        Color tint;
    };
    struct TextParams {
        std::string text;
        Vector2 position;
        int fontSize;
        Color color;
    };
    using DrawCommand = std::variant<CircleParams, TextureParams, TextParams>;

    class Renderer : public IRenderer {
    public:
        Renderer() = default;
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        ~Renderer() override = default;

        [[nodiscard]] std::expected<void, std::string> initialize(int width, int height, const std::string& title) override;
        void shutdown() override;

        void beginFrame() override;
        void endFrame() override;
        [[nodiscard]] bool shouldClose() const override;

        void setRenderTarget(RenderTexture2D& renderTarget) override;
        void clearRenderTarget() override;

        // Queueing API
        void queueDraw(const DrawCommand& cmd);

        // These methods now queue draw commands
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
        int windowWidth = 0;
        int windowHeight = 0;
        std::unique_ptr<RenderTexture2D> renderTexture;
        Vector2 screenCenter = {0, 0};
        bool windowShouldClose = false;
        std::vector<DrawCommand> drawQueue;
    };
}

#endif// RENDERER_H
