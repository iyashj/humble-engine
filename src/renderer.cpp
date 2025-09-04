#include "../include/renderer.h"

namespace engine {

	void Renderer::initialise(int width, int height, const std::string& title) {
		InitWindow(width, height, title.c_str());
		SetTargetFPS(60);
	}
	void Renderer::shutdown() {
		if (renderTexture != nullptr) {
			UnloadRenderTexture(*renderTexture);
			renderTexture = nullptr;
		}
		CloseWindow();
	}

	void Renderer::beginFrame() {
		BeginDrawing();
		if (renderTexture != nullptr) {
			BeginTextureMode(*renderTexture);
		}
	}
	void Renderer::endFrame() {
		if(renderTexture != nullptr) {
			EndTextureMode();
		}
	}
	bool Renderer::shouldClose() const {
		return WindowShouldClose();
	}

	void Renderer::setRenderTarget(RenderTexture2D &renderTarget) {
		renderTexture = &renderTarget;
	}
	void Renderer::clearRenderTarget() {
		renderTexture = nullptr;
	}

	void Renderer::drawCircle(const Vector2 &position, float radius, const Color &color) {
		DrawCircleV(position, radius, color);
	}
	void Renderer::drawTexture(const Texture2D &texture, const Rectangle &source, const Rectangle &dest,
							   const Vector2 &origin, float rotation, const Color &tint) {
		DrawTexturePro(texture, source, dest, origin, rotation, tint);
	}
	void Renderer::drawText(const std::string &text, const Vector2 &position, int fontSize, const Color &color) {
		DrawText(text.c_str(), static_cast<int>(position.x), static_cast<int>(position.y), fontSize, color);
	}

	int Renderer::getWindowWidth() const { return GetScreenWidth(); }
	int Renderer::getWindowHeight() const { return GetScreenHeight(); }
	RenderTexture2D & Renderer::getRenderTexture() { return *renderTexture; }
	const Vector2 Renderer::getScreenCenter() const {
		return Vector2{
		static_cast<float>(getWindowWidth()) / 2.0f,
		static_cast<float>(getWindowHeight()) / 2.0f};
	}
}
