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
        using ErasedAddFn = std::function<void(EntityId, const void *)>;
        using ErasedRemoveFn = std::function<void(EntityId)>;

    public:
        ComponentEventBus() {
            m_on_component_add_events = std::unordered_map<std::type_index, std::vector<ErasedAddFn> >();
            m_on_component_remove_events = std::unordered_map<std::type_index, std::vector<ErasedRemoveFn> >();
        }

        ~ComponentEventBus() = default;

        template<typename T>
        void SubscribeOnComponentAddEvent(std::function<void(EntityId, const T &)> fn) {
            const auto type_index = std::type_index(typeid(T));
            auto &vec = m_on_component_add_events[type_index];
            vec.emplace_back(Wrap<T>(std::move(fn)));
        }

        template<typename T>
        void SubscribeOnComponentRemoveEvent(std::function<void(EntityId)> fn) {
            const auto type_index = std::type_index(typeid(T));
            auto &vec = m_on_component_remove_events[type_index];
            vec.emplace_back(std::move(fn));
        }

        template<typename T>
        void RaiseAddComponentEvent(const EntityId entity, const T &value) const {
            const auto it = m_on_component_add_events.find(std::type_index(typeid(T)));
            if (it == m_on_component_add_events.end()) {
                return;
            }
            for (auto &func: it->second) {
                if (func) {
                    func(entity, &value);
                }
            }
        }

        template<typename T>
        void RaiseRemoveComponentEvent(const EntityId entity) const {
            const auto it = m_on_component_remove_events.find(std::type_index(typeid(T)));
            if (it == m_on_component_remove_events.end()) {
                return;
            }
            for (auto &func: it->second) {
                if (func) {
                    func(entity);
                }
            }
        }

    private:
        std::unordered_map<std::type_index, std::vector<ErasedAddFn> > m_on_component_add_events;
        std::unordered_map<std::type_index, std::vector<ErasedRemoveFn> > m_on_component_remove_events;

        template<typename T>
        static ErasedAddFn Wrap(std::function<void(EntityId, const T &)> fn) {
            return [fn = std::move(fn)](EntityId entity, const void *data) {
                fn(entity, *static_cast<const T *>(data));
            };
        }
    };
}
