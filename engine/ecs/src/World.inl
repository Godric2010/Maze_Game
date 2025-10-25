#pragma once
#include "ComponentManager.hpp"

namespace Engine::Ecs {
    struct World::WorldImpl {
        std::unique_ptr<EntityManager> entity_manager;
        std::unique_ptr<ComponentManager> component_manager;
    };

    inline World::World() : m_impl(std::make_unique<WorldImpl>()) {
        m_impl->entity_manager = std::make_unique<EntityManager>();
        m_impl->component_manager = std::make_unique<ComponentManager>();
        m_ecs_event_buffer = std::make_unique<Buffer::EventBuffer<EcsEvent> >();
        m_physics_event_buffer = std::make_unique<Buffer::EventBuffer<PhysicsEvent> >();
        m_component_event_bus = std::make_unique<ComponentEventBus>();
        m_physics_event_bus = std::make_unique<PhysicsEventBus>();
    }

    inline World::~World() = default;

    inline EntityId World::CreateEntity() const {
        const auto entity = m_impl->entity_manager->ReserveEntity();
        const EcsEvent cmd{EcsEventType::CreateEntity, entity};
        m_ecs_event_buffer->EnqueueEvent(cmd);
        return entity;
    }

    inline void World::DestroyEntity(const EntityId entity) const {
        const EcsEvent cmd{EcsEventType::DestroyEntity, entity};
        m_ecs_event_buffer->EnqueueEvent(cmd);
    }


    template<typename T>
    void World::AddComponent(EntityId entity, T component) {
        if (!m_impl->entity_manager->IsEntityAlive(entity) &&
            !m_impl->entity_manager->IsEntityPending(entity)) {
            return;
        }

        const auto id = m_impl->component_manager->RegisterType<T>();

        EcsEvent cmd{EcsEventType::AddComponent, entity, id};
        cmd.payload.resize(sizeof(T));
        std::memcpy(cmd.payload.data(), &component, sizeof(T));
        m_ecs_event_buffer->EnqueueEvent(std::move(cmd));
    }

    template<typename T>
    void World::RemoveComponent(const EntityId entity) const {
        if (!m_impl->entity_manager->IsEntityAlive(entity) &&
            !m_impl->entity_manager->IsEntityPending(entity)) {
            return;
        }

        const auto id = m_impl->component_manager->GetComponentTypeId<T>();
        EcsEvent cmd{EcsEventType::RemoveComponent, entity, id};
        m_ecs_event_buffer->EnqueueEvent(std::move(cmd));
    }

    inline void World::ApplyCommands() const {
        for (const auto [type, target_entity, other_collider_entity]: m_physics_event_buffer->Get()) {
            switch (type) {
                case PhysicsEventType::OnCollisionEnter:
                    m_physics_event_bus->RaiseOnCollisionEnter(target_entity, other_collider_entity);
                    break;
                case PhysicsEventType::OnCollisionExit:
                    m_physics_event_bus->RaiseOnCollisionExit(target_entity, other_collider_entity);
                    break;
                case PhysicsEventType::OnTriggerEnter:
                    m_physics_event_bus->RaiseOnTriggerEnter(target_entity, other_collider_entity);
                    break;
                case PhysicsEventType::OnTriggerExit:
                    m_physics_event_bus->RaiseOnTriggerExit(target_entity, other_collider_entity);
                    break;
            }
        }
        m_physics_event_buffer->ClearEvents();

        for (const auto &[type, entity, component_type_id, payload]: m_ecs_event_buffer->Get()) {
            switch (type) {
                case EcsEventType::CreateEntity: {
                    m_impl->entity_manager->CommitEntity(entity);
                    break;
                }
                case EcsEventType::DestroyEntity: {
                    m_impl->component_manager->OnDestroyEntity(entity);
                    m_impl->entity_manager->DestroyEntity(entity);
                    break;
                }
                case EcsEventType::AddComponent: {
                    const ComponentMeta component_meta = m_impl->component_manager->GetComponentMeta(component_type_id);
                    m_impl->component_manager->AddById(entity, component_type_id, payload.data());
                    if (component_meta.on_add_event) {
                        component_meta.on_add_event(*m_component_event_bus, entity, payload.data());
                    }

                    break;
                }
                case EcsEventType::RemoveComponent: {
                    const ComponentMeta component_meta = m_impl->component_manager->GetComponentMeta(component_type_id);
                    m_impl->component_manager->RemoveById(entity, component_type_id);
                    if (component_meta.on_remove_event) {
                        component_meta.on_remove_event(*m_component_event_bus, entity);
                    }
                    break;
                }
                case EcsEventType::UpdateComponent: {
                    m_impl->component_manager->SetById(entity, component_type_id, payload.data());
                }
                default:
                    throw std::runtime_error("Unhandled command type");
            }
        }
        m_ecs_event_buffer->ClearEvents();
    }

    template<typename T>
    T *World::GetComponent(const EntityId entity) {
        if (!m_impl->entity_manager->IsEntityAlive(entity)) {
            return nullptr;
        }

        if (!m_impl->component_manager->HasComponent<T>(entity)) {
            return nullptr;
        }

        return m_impl->component_manager->GetComponent<T>(entity);
    }

    template<typename T>
    std::vector<std::pair<T *, EntityId> > World::GetComponentsOfType() {
        const auto entities = m_impl->entity_manager->GetAllActiveEntities();
        std::vector<std::pair<T *, EntityId> > components;
        for (const auto &entity: entities) {
            if (m_impl->component_manager->HasComponent<T>(entity)) {
                auto component = m_impl->component_manager->GetComponent<T>(entity);
                components.emplace_back(std::make_pair(component, entity));
            }
        }
        return components;
    }
} // namespace
