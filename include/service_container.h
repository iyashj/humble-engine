#ifndef SERVICE_CONTAINER_H
#define SERVICE_CONTAINER_H
#include <memory>
#include <unordered_map>
#include <typeindex>

namespace engine {
class ServiceContainer {
public:
    template<typename T, typename Impl, typename... Args>
    void registerService(Args&&... args) {
        services_[std::type_index(typeid(T))] = std::make_shared<Impl>(std::forward<Args>(args)...);
    }

    template<typename T>
    std::shared_ptr<T> get() const {
        if (const auto it = services_.find(std::type_index(typeid(T))); it != services_.end())
            return std::static_pointer_cast<T>(it->second);
        return nullptr;
    }
private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> services_;
};
}
#endif // SERVICE_CONTAINER_H
