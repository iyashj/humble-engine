#ifndef BASE_LAYER_IFILE_MANAGER_H
#define BASE_LAYER_IFILE_MANAGER_H

#include <expected>
#include <raylib.h>
#include <string>

class IFileManager {
public:
    virtual std::expected<Texture2D, std::string> loadTexture(const std::string& filepath) = 0;
    virtual std::expected<Sound, std::string> loadSound(const std::string& filepath) = 0;
    virtual std::expected<Shader, std::string> loadShader(const std::string& vertexPath, const std::string& fragmentPath) = 0;

    [[nodiscard]] virtual bool isTextureLoaded(const std::string& filepath) const = 0;
    [[nodiscard]] virtual bool isSoundLoaded(const std::string& filepath) const = 0;
    [[nodiscard]] virtual bool isShaderLoaded(const std::string& shaderKey) const = 0;

    virtual ~IFileManager() = default;
};

#endif
