#pragma once
#include <memory>
#include <functional>
#include <vector>
#include <any>
#include "Camera.hpp"
#include "GameWorld.hpp"
#include "IEngine.hpp"
#include "ServiceLocator.hpp"
#include "SystemManager.hpp"
#include "../ecs/include/World.hpp"
#include "../renderer/RenderController.hpp"
#include "../environment/include/Window.hpp"
#include "../environment/include/Input.hpp"

namespace Engine::Core {
    /**
     * @class EngineController
     * The central engine controller that brings all elements of the engine together.
     * All systems and their events are managed through this instance.
     */
    class EngineController final : public IEngine {
    public:
        EngineController();

        ~EngineController() override;

        /**
         * Initialize the engine backend
         */
        void Initialize(const std::vector<Ecs::SystemMeta> &systems);

        /**
         * Update the engines systems, like drawing, the objects in the world, etc.
         */
        void Update() const;

        /**
         * Shutdown the engines system and free all resources.
         */
        void Shutdown() const;

        [[nodiscard]] GameWorld &GetWorld() const override;

        Renderer::MeshHandle RegisterMesh(const Renderer::MeshAsset &mesh_asset) override;

        void RegisterForSystemCommands(std::function<void(std::vector<std::any>)> command_callback) override;

    private:
        std::unique_ptr<ServiceLocator> m_services;
        std::unique_ptr<Environment::IWindow> m_window;
        std::unique_ptr<Ecs::World> m_world;
        std::unique_ptr<GameWorld> m_game_world;
        std::unique_ptr<Ecs::SystemManager> m_system_manager;
    };
} // namespace
