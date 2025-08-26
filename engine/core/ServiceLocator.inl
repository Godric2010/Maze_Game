#include "ServiceLocator.hpp"

namespace Engine::Core {
    inline ServiceLocator::ServiceLocator() = default;

    inline ServiceLocator::~ServiceLocator() = default;

    template<typename T>
    void ServiceLocator::RegisterService(std::unique_ptr<T> service) {
        auto typeIndex = std::type_index(typeid(T));
        if (const auto it = m_services.find(typeIndex); it == m_services.end()) {
            m_services.insert({typeIndex, std::make_shared<Box<T> >(std::move(service))});
        }
    }

    template<typename T>
    T *ServiceLocator::TryGetService() const {
        const auto typeIndex = std::type_index(typeid(T));
        const auto it = m_services.find(typeIndex);
        if (it == m_services.end()) {
            return nullptr;
        }
        auto box = std::dynamic_pointer_cast<Box<T>>(it->second);
        return box ? box->ptr.get(): nullptr;
    }

    inline void *ServiceLocator::TryGetServiceRaw(std::type_index type) {
        const auto it = m_services.find(type);
        if (it == m_services.end()) {
            return nullptr;
        }
        const auto box = it->second;
        return box ? box->GetRaw() : nullptr;
    }

    inline const void *ServiceLocator::TryGetServiceRaw(std::type_index type) const {
        const auto it = m_services.find(type);
        if (it == m_services.end()) {
            return nullptr;
        }
        const auto box = std::dynamic_pointer_cast<Box<const void *>>(it->second);
        return box ? box->ptr.get() : nullptr;
    }

} // namespace
