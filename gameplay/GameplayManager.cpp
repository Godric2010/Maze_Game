#include "GameplayManager.hpp"

#include <iostream>

#include "GameScene.hpp"
#include "Mesh.hpp"
#include "commands/PauseCommand.hpp"
#include "commands/QuitCommand.hpp"
#include "commands/ui/ButtonClickedCommand.hpp"
#include "components/Transform.hpp"
#include "ui/Button.hpp"
#include "ui/Image.hpp"
#include "ui/RectTransform.hpp"

namespace Gameplay {
    GameplayManager::GameplayManager(Engine::Core::IEngine &engine) : m_engine(engine) {
        m_mesh_handler = std::make_unique<MeshHandler>(engine);

        const Engine::Core::InputMap player_input_map{
            .name = "PlayerInputMap",
            .key_bindings = {
                Engine::Core::KeyBinding{
                    .name = "forward",
                    .key = Engine::Environment::Key::W,
                    .press_state = Engine::Environment::PressState::Pressed
                },
                Engine::Core::KeyBinding{
                    .name = "backwards",
                    .key = Engine::Environment::Key::S,
                    .press_state = Engine::Environment::PressState::Pressed
                },
                Engine::Core::KeyBinding{
                    .name = "left",
                    .key = Engine::Environment::Key::A,
                    .press_state = Engine::Environment::PressState::Pressed
                },
                Engine::Core::KeyBinding{
                    .name = "right",
                    .key = Engine::Environment::Key::D,
                    .press_state = Engine::Environment::PressState::Pressed
                },
                Engine::Core::KeyBinding{
                    .name = "pause",
                    .key = Engine::Environment::Key::Esc,
                    .press_state = Engine::Environment::PressState::Down
                }
            },
            .mouse_bindings = {},
            .mouse_visible = false,
        };
        m_engine.RegisterInputMap(player_input_map);

        const Engine::Core::InputMap pause_input_map{
            .name = "PauseInputMap",
            .key_bindings = {
                Engine::Core::KeyBinding{
                    .name = "resume",
                    .key = Engine::Environment::Key::Esc,
                    .press_state = Engine::Environment::PressState::Down
                },
            },
            .mouse_bindings = {
                Engine::Core::MouseKeyBinding{
                    .name = "confirm",
                    .button = Engine::Environment::MouseButton::Left,
                    .press_state = Engine::Environment::PressState::Up
                }
            },
            .mouse_visible = true,
        };
        m_engine.RegisterInputMap(pause_input_map);
    }

    GameplayManager::~GameplayManager() = default;

    void GameplayManager::Initialize() {
        auto scene = std::make_unique<GameScene>(m_mesh_handler.get());
        m_engine.LoadScene(std::move(scene));
    }

    void GameplayManager::Shutdown() {
    }
} // namespace
