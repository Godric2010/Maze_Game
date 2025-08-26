#pragma once
#include <typeindex>
#include <unordered_map>
#include "IServiceToEcsProvider.hpp"

namespace Engine::Core {
    class ServiceLocator : public Ecs::IServiceToEcsProvider {
    public:
        ServiceLocator();

        ~ServiceLocator();

        template<class T>
        void RegisterService(std::unique_ptr<T> service);

        template<class T>
        T *TryGetService() const;

        void *TryGetServiceRaw(std::type_index type) override;

        [[nodiscard]] const void *TryGetServiceRaw(std::type_index type) const override;

    private:
        struct BoxBase {
            virtual ~BoxBase() = default;
            virtual void* GetRaw() = 0;
         };

        template<class T>
        struct Box final : BoxBase {
            std::shared_ptr<T> ptr;

            void *GetRaw() override {
                return ptr.get();
            };

            explicit Box(std::shared_ptr<T> ptr) : ptr(std::move(ptr)) {
            }
        };

        std::unordered_map<std::type_index, std::shared_ptr<BoxBase> > m_services;
    };
} // namespace

#include <ServiceLocator.inl>
