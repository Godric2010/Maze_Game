//
// Created by Sebastian Borsch on 29.09.25.
//

#pragma once
#include <functional>
#include <typeindex>
#include <unordered_map>

#include "../src/Entity.hpp"


namespace Engine::Ecs {
    class ComponentEventBus {
    private:
        using ErasedFn = std::function<void(EntityId, const void *)>;
    public:
        ComponentEventBus() {
            m_buckets = std::unordered_map<std::type_index, std::vector<ErasedFn>>();
        }

        ~ComponentEventBus() = default;

        template<typename T>
        void Subscribe(std::function<void(EntityId, const T &)> fn) {
            const auto type_index = std::type_index(typeid(T));
            auto &vec = m_buckets[type_index];
            vec.emplace_back(Wrap<T>(std::move(fn)));
        }

        template<typename T>
        void RaiseAddComponentEvent(const EntityId entity, const T &value) const {
            const auto it = m_buckets.find(std::type_index(typeid(T)));
            if (it == m_buckets.end()) {
                return;
            }
            for (auto &func: it->second) {
                if (func) {
                    func(entity, &value);
                }
            }
        }

        // TODO: This uses the same event as the add... consider using another func for this in order to keep things clean and avoid errors in the future
        template<typename T>
        void RaiseRemoveComponentEvent(const EntityId entity, const void *bytes) const {
            const auto it = m_buckets.find(std::type_index(typeid(T)));
            if (it == m_buckets.end()) {
                return;
            }
            for (auto &func: it->second) {
                if (func) {
                    func(entity, &bytes);
                }
            }
        }

    private:
        std::unordered_map<std::type_index, std::vector<ErasedFn> > m_buckets;

        template<typename T>
        static ErasedFn Wrap(std::function<void(EntityId, const T &)> fn) {
            return [fn = std::move(fn)](EntityId entity, const void *data) {
                fn(entity, *static_cast<const T *>(data));
            };
        }
    };
}
