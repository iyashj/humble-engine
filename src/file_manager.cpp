#include "file_manager.h"
#include <iostream>
#include <ranges>
#include <expected>

namespace engine {
    std::expected<Texture2D, std::string> FileManager::loadTexture(const std::string& filepath) {
        if (const auto it = textures.find(filepath); it != textures.end()) {
            return it->second;
        }
        const Texture2D texture = LoadTexture(filepath.c_str());
        if (texture.id == 0) {
            return std::unexpected(std::format("Failed to load texture {}", filepath));
        }
        textures[filepath] = texture;
        std::cout << std::format("Loaded texture: {}\n", filepath);
        return texture;
    }
    std::expected<Sound, std::string> FileManager::loadSound(const std::string& filepath) {
        if (const auto it = sounds.find(filepath); it != sounds.end()) {
            return it->second;
        }
        const Sound sound = LoadSound(filepath.c_str());
        if (sound.stream.buffer == nullptr) {
            return std::unexpected(std::format("Failed to load sound: {}", filepath));
        }
        sounds[filepath] = sound;
        std::cout << "Loaded sound: " << filepath << std::endl;
        return sound;
    }
    std::expected<Shader, std::string> FileManager::loadShader(const std::string& vertexPath, const std::string& fragmentPath) {
        const auto shaderKey = getShaderKey(vertexPath, fragmentPath);
        if (const auto it = shaders.find(shaderKey); it != shaders.end()) {
            return it->second;
        }
        const Shader shader = LoadShader(vertexPath.c_str(), fragmentPath.c_str());
        if (shader.id == 0) {
            return std::unexpected(std::format("Failed to load shader: {} | {}", vertexPath, fragmentPath));
        }
        shaders[shaderKey] = shader;
        std::cout << "Loaded shader: " << shaderKey << std::endl;
        return shader;
    }
    bool FileManager::unloadTexture(const std::string& filepath) {
        if (const auto it = textures.find(filepath); it != textures.end()) {
            UnloadTexture(it->second);
            textures.erase(it);
            std::cout << "Unloaded texture: " << filepath << std::endl;
            return true;
        }
        return false;
    }
    bool FileManager::unloadSound(const std::string& filepath) {
        if (const auto it = sounds.find(filepath); it != sounds.end()) {
            UnloadSound(it->second);
            sounds.erase(it);
            std::cout << "Unloaded sound: " << filepath << std::endl;
            return true;
        }
        return false;
    }
    bool FileManager::unloadShader(const std::string& shaderKey) {
        if (const auto it = shaders.find(shaderKey); it != shaders.end()) {
            UnloadShader(it->second);
            shaders.erase(it);
            std::cout << "Unloaded shader: " << shaderKey << std::endl;
            return true;
        }
        return false;
    }
    void FileManager::unloadAll() {
        for (const auto &val: textures | std::views::values) {
            UnloadTexture(val);
        }
        textures.clear();
        for (const auto &val: sounds | std::views::values) {
            UnloadSound(val);
        }
        sounds.clear();
        for (const auto &val: shaders | std::views::values) {
            UnloadShader(val);
        }
        shaders.clear();
        std::cout << "Unloaded all resources" << std::endl;
    }
    bool FileManager::isTextureLoaded(const std::string& filepath) const {
        return textures.contains(filepath);
    }
    bool FileManager::isSoundLoaded(const std::string& filepath) const {
        return sounds.contains(filepath);
    }
    bool FileManager::isShaderLoaded(const std::string& shaderKey) const {
        return shaders.contains(shaderKey);
    }
    std::string FileManager::getShaderKey(const std::string& vertexPath, const std::string& fragmentPath) {
        return std::format("{}|{}", vertexPath, fragmentPath);
    }
    FileManager::~FileManager() {
        unloadAll();
    }
}
