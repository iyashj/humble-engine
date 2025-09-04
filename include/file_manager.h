#ifndef BASE_LAYER_FILE_MANAGER_H
#define BASE_LAYER_FILE_MANAGER_H
#include <raylib.h>
#include <string>
#include <unordered_map>
#include <expected>
#include "ifilemanager.h"

namespace engine {
    class FileManager : public IFileManager {
    public:
        FileManager() = default;
        ~FileManager() override;

        std::expected<Texture2D, std::string> loadTexture(const std::string& filepath) override;
        std::expected<Sound, std::string> loadSound(const std::string& filepath) override;
        std::expected<Shader, std::string> loadShader(const std::string& vertexPath, const std::string& fragmentPath) override;

        void unloadAll();

        [[nodiscard]] bool isTextureLoaded(const std::string& filepath) const override;
        [[nodiscard]] bool isSoundLoaded(const std::string& filepath) const override;
        [[nodiscard]] bool isShaderLoaded(const std::string& shaderKey) const override;
    private:
        std::unordered_map<std::string, Texture2D> textures;
        std::unordered_map<std::string, Sound> sounds;
        std::unordered_map<std::string, Shader> shaders;

        bool unloadTexture(const std::string& filepath);
        bool unloadSound(const std::string& filepath);
        bool unloadShader(const std::string& shaderKey);

        [[nodiscard]] static std::string getShaderKey(const std::string &vertexPath, const std::string &fragmentPath);
    };
}

#endif
