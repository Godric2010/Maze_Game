#include "GameplayManager.hpp"

#include "GameEndScene.hpp"
#include "GameScene.hpp"
#include "MainMenuScene.hpp"

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
        };
        m_engine.RegisterInputMap(pause_input_map);
    }

    GameplayManager::~GameplayManager() = default;

    void GameplayManager::Initialize() const {
        m_engine.RegisterScene("MainMenu", [](const Engine::Core::SceneArgs &args) {
            const auto &mesh_handler = std::any_cast<MeshHandler *>(args.payload);
            return std::make_unique<MainMenuScene>(mesh_handler);
        });

        m_engine.RegisterScene("Game", [](const Engine::Core::SceneArgs &args) {
            const auto game_scene_settings = std::any_cast<GameSceneSettings>(args.payload);
            return std::make_unique<GameScene>(game_scene_settings);
        });

        m_engine.RegisterScene("GameEnd", [](const Engine::Core::SceneArgs &args) {
            const auto game_end_data = std::any_cast<GameEndShowData>(args.payload);
            return std::make_unique<GameEndScene>(game_end_data);
        });

        m_engine.SetInitialScene("MainMenu", Engine::Core::SceneArgs{m_mesh_handler.get()});
    }

    void GameplayManager::Shutdown() {
    }
} // namespace
