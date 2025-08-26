//
// Created by Sebastian Borsch on 26.08.25.
//

#pragma once
#include <typeindex>

namespace Engine::Ecs{
    class IServiceToEcsProvider{
    public:

        virtual ~IServiceToEcsProvider() = default;

        virtual void* TryGetServiceRaw(std::type_index type) =0;
        [[nodiscard]] virtual const void* TryGetServiceRaw(std::type_index type) const = 0;

        template<class T>
        T* GetService() {return static_cast<T*>(TryGetServiceRaw(typeid(T)));};

        template<class T>
        const T* GetServiceConst() const {return static_cast<const T*>(TryGetServiceRaw(typeid(T)));};
    };
}
