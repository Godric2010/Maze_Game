#pragma once
#include <vector>

#include "ComponentManager.hpp"
#include "Entity.hpp"

namespace Engine::Ecs {

    enum class CommandType {
        CreateEntity,
        DestroyEntity,
        AddComponent,
        RemoveComponent,
        UpdateComponent,
    };

    struct Command {
        CommandType type;
        EntityId entity;

        ComponentTypeId component_type_id;
        std::vector<std::byte> payload;
    };

    class CommandBuffer {
    public:
        CommandBuffer();

        ~CommandBuffer();

        void Enqueue(Command command);

        void Clear();

        [[nodiscard]] const std::vector<Command>& GetCommands() const;

    private:
        std::vector<Command> m_commands;
    };
} // namespace