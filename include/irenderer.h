#ifndef IRENDERER_H
#define IRENDERER_H
#include <string>
#include <raylib.h>

namespace engine {

    class IFrameHandler {
    public:
        virtual ~IFrameHandler() = default;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;
        [[nodiscard]] virtual bool shouldClose() const = 0;
    };

    class IRenderTargetHandler {
    public:
        virtual ~IRenderTargetHandler() = default;

        virtual void setRenderTarget(RenderTexture2D& renderTarget) = 0;
        virtual void clearRenderTarget() = 0;
    };

    class IWindowHandler {
    public:
        virtual ~IWindowHandler() = default;

        [[nodiscard]] virtual int getWindowHeight() const = 0;
        [[nodiscard]] virtual int getWindowWidth() const = 0;
        [[nodiscard]] virtual RenderTexture2D& getRenderTexture() = 0;
        [[nodiscard]] virtual const Vector2 getScreenCenter() const = 0;
    };

    class IDrawHandler {
    public:
        virtual void drawCircle(
            const Vector2& position,
            float radius,
            const Color& color) = 0;

        virtual void drawTexture(const Texture2D& texture,
            const Rectangle& source,
            const Rectangle& dest,
            const Vector2& origin,
            float rotation,
            const Color& tint) = 0;

        virtual void drawText(const std::string& text,
            const Vector2& position,
            int fontSize,
            const Color& color) = 0;

        virtual ~IDrawHandler() = default;
    };


    class IRenderer : public IDrawHandler, public IWindowHandler, public IFrameHandler, public IRenderTargetHandler
    {
    public:
        ~IRenderer() override = default;

        virtual void initialise(int width, int height, const std::string& title) = 0;
        virtual void shutdown() = 0;
    };
}
#endif // IRENDERER_H
