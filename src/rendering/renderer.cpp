#include "rendering/renderer.h"
#include <fmt/core.h>
#include <variant>

namespace engine {

	std::expected<void, std::string> Renderer::initialize(int width, int height, const std::string& title) {
		windowWidth = width;
		windowHeight = height;
		screenCenter = Vector2{
			static_cast<float>(windowWidth) / 2.0f,
			static_cast<float>(windowHeight) / 2.0f};
		InitWindow(windowWidth, windowHeight, title.c_str());
		SetTargetFPS(60);
		try {
			renderTexture = std::make_unique<RenderTexture2D>(LoadRenderTexture(windowWidth, windowHeight));
		} catch (const std::exception& e) {
			return std::unexpected(fmt::format("Failed to create render texture: {}", e.what()));
		}
		return {};
	}
	void Renderer::shutdown() {
		if (renderTexture) {
			UnloadRenderTexture(*renderTexture);
			renderTexture.reset();
		}
		CloseWindow();
	}

	void Renderer::beginFrame() {
		BeginDrawing();
		if (renderTexture) {
			BeginTextureMode(*renderTexture);
		}
	}

	void Renderer::endFrame() {
		// Flush draw queue (queue rendering)
		struct DrawVisitor {
			void operator()(const CircleParams& params) const {
				DrawCircleV(params.position, params.radius, params.color);
			}
			void operator()(const TextureParams& params) const {
				if (params.texture != nullptr) {
					DrawTexturePro(*params.texture, params.source, params.dest, params.origin, params.rotation, params.tint);
				}
			}
			void operator()(const TextParams& params) const {
				DrawText(params.text.c_str(), static_cast<int>(params.position.x), static_cast<int>(params.position.y), params.fontSize, params.color);
			}
		};
		for (const auto& cmd : drawQueue) {
			DrawVisitor visitor;
			std::visit(visitor, cmd);
		}

		drawQueue.clear();
		if (renderTexture) {
			EndTextureMode();
		}
		EndDrawing();
	}
	bool Renderer::shouldClose() const { return WindowShouldClose(); }

	void Renderer::setRenderTarget(RenderTexture2D &renderTarget) {
		renderTexture = std::make_unique<RenderTexture2D>(renderTarget);
	}
	void Renderer::clearRenderTarget() {
		renderTexture.reset();
	}

	void Renderer::queueDraw(const DrawCommand& cmd) {
		drawQueue.push_back(cmd);
	}

	void Renderer::drawCircle(const Vector2& position, float radius, const Color& color) {
		CircleParams params{position, radius, color};
		queueDraw(params);
	}
	void Renderer::drawTexture(const Texture2D& texture, const Rectangle& source, const Rectangle& dest,
							   const Vector2& origin, float rotation, const Color& tint) {
		TextureParams params{&texture, source, dest, origin, rotation, tint};
		queueDraw(params);
	}
	void Renderer::drawText(const std::string& text, const Vector2& position, int fontSize, const Color& color) {
		TextParams params{text, position, fontSize, color};
		queueDraw(params);
	}

	int Renderer::getWindowWidth() const { return windowWidth; }
	int Renderer::getWindowHeight() const { return windowHeight; }
	RenderTexture2D& Renderer::getRenderTexture() { return *renderTexture; }
	const Vector2 Renderer::getScreenCenter() const { return screenCenter; }



}
