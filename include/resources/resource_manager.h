#ifndef HUMBLEENGINE_RESOURCE_MANAGER_H
#define HUMBLEENGINE_RESOURCE_MANAGER_H
#include <expected>
#include <fmt/format.h>
#include <format>
#include <raylib.h>
#include <shared_mutex>
#include <string>
#include <unordered_map>

namespace engine::resources {
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
            Texture2D texture = LoadTexture(path.c_str());
            if (texture.id == 0) {
                return std::unexpected(fmt::format("Failed to load texture: {}", path));
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
            Sound sound = LoadSound(path.c_str());
            if (sound.stream.buffer == nullptr) {
                return std::unexpected(fmt::format("Failed to load sound: {}", path));
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