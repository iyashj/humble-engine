#ifndef HUMBLEENGINE_RESOURCE_MANAGER_H
#define HUMBLEENGINE_RESOURCE_MANAGER_H
#include <expected>
#include <filesystem>
#include <fmt/format.h>
#include <format>
#include <iostream>
#include <raylib.h>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
#elif defined(__linux__)
    #include <unistd.h>
#endif

namespace engine::resources {

    inline std::filesystem::path getExecutableDir()
{
    std::filesystem::path exePath;

    #if defined(_WIN32)
    char buffer[MAX_PATH];
    DWORD len = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (len > 0 && len < MAX_PATH) {
        exePath = std::filesystem::path(buffer).parent_path();
    }
    #elif defined(__APPLE__)
    char buffer[1024];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0) {
        exePath = std::filesystem::path(buffer).parent_path();
    }
    #elif defined(__linux__)
    char buffer[1024];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        exePath = std::filesystem::path(buffer).parent_path();
    }
    #endif

    return exePath;
}

    inline std::filesystem::path& resourceRoot() {
        static std::filesystem::path root;
        return root;
    }

    inline void setResourceRoot(std::filesystem::path root) {
        std::cout << "Setting resource root to %s" << root.c_str() << '\n';
        resourceRoot() = std::move(root);
    }

    inline std::filesystem::path resolve(const std::string& relative) {
        auto abs = resourceRoot() / relative;
        if (!std::filesystem::exists(abs)) {
            throw std::runtime_error("Resource not found: " + abs.string());
        }
        return abs;
    }

    template <typename T>
    concept Resource = requires
    {
        typename T::handle_type;
        {T::load(std::string {} )} -> std::convertible_to<std::expected<typename T::handle_type, std::string>>;
        {T::unload(typename T::handle_type {} )} -> std::same_as<void>;
    };

    template<Resource T>
   class ResourceCache {
    public:
        std::expected<typename T::handle_type, std::string> get(const std::string &path) {
            std::shared_lock lock(mutex);
            if (auto it = cache.find(path); it != cache.end()) {
                return it->second;
            }
            lock.unlock();

            std::unique_lock write_lock(mutex);
            if (auto it = cache.find(path); it != cache.end()) {
                return it->second;
            }

            auto result = T::load(path);
            if (result.has_value()) {
                cache[path] = result.value();
            }

            return result;
        }

        void clear() {
            std::unique_lock lock(mutex);
            for (auto& [path, resource] : cache) {
                T::unload(resource);
            }

            cache.clear();
        }

        ~ResourceCache() { clear(); }

    private:
        std::unordered_map<std::string, typename T::handle_type> cache;
        mutable std::shared_mutex mutex;
    };

    struct TextureResource {
        using handle_type = Texture2D;

        static std::expected<handle_type, std::string> load(const std::string &path) {
            const std::string resolvedPath = resolve(path);
            Texture2D texture = LoadTexture(resolvedPath.c_str());
            if (texture.id == 0) {
                return std::unexpected(fmt::format("Failed to load texture: {} (resolved to: {})", path, resolvedPath));
            }
            return texture;
        }

        static void unload(const Texture2D& texture) {
            UnloadTexture(texture);
        }
    };

    struct SoundResource {
        using handle_type = Sound;

        static std::expected<handle_type, std::string> load(const std::string &path) {
            const std::string resolvedPath = resolve(path);
            Sound sound = LoadSound(resolvedPath.c_str());
            if (sound.stream.buffer == nullptr) {
                return std::unexpected(fmt::format("Failed to load sound: {} (tried: {})", path, resolvedPath));
            }

            return sound;
        }

        static void unload(const Sound& sound) {
            UnloadSound(sound);
        }
    };

    inline auto& textures() {
        static ResourceCache<TextureResource> cache;
        return cache;
    }

    inline auto& sounds() {
        static ResourceCache<SoundResource> cache;
        return cache;
    }

    inline std::expected<Texture2D, std::string> loadTexture(const std::string& path) {
        return textures().get(path);
    }

    inline std::expected<Sound, std::string> loadSound(const std::string& path) {
        return sounds().get(path);
    }
}

#endif //HUMBLEENGINE_RESOURCE_MANAGER_H