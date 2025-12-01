#include "GameplayManager.hpp"

#include "GameEndScene.hpp"
#include "GameScene.hpp"
#include "MainMenuScene.hpp"

namespace Gameplay {
    GameplayManager::GameplayManager(Engine::Core::IEngine& engine) : m_engine(engine) {
        m_mesh_handler = std::make_unique<MeshHandler>(engine);

        const Engine::Input::InputMap player_input_map{
            .name = "PlayerInputMap",
            .key_bindings = {
                Engine::Input::KeyBinding{
                    .name = "forward",
                    .key = Engine::Input::Key::W,
                    .press_state = Engine::Input::PressState::Pressed
                },
                Engine::Input::KeyBinding{
                    .name = "backwards",
                    .key = Engine::Input::Key::S,
                    .press_state = Engine::Input::PressState::Pressed
                },
                Engine::Input::KeyBinding{
                    .name = "left",
                    .key = Engine::Input::Key::A,
                    .press_state = Engine::Input::PressState::Pressed
                },
                Engine::Input::KeyBinding{
                    .name = "right",
                    .key = Engine::Input::Key::D,
                    .press_state = Engine::Input::PressState::Pressed
                },
                Engine::Input::KeyBinding{
                    .name = "pause",
                    .key = Engine::Input::Key::Esc,
                    .press_state = Engine::Input::PressState::Down
                }
            },
            .mouse_bindings = {},
        };
        m_engine.RegisterInputMap(player_input_map);

        const Engine::Input::InputMap pause_input_map{
            .name = "PauseInputMap",
            .key_bindings = {
                Engine::Input::KeyBinding{
                    .name = "resume",
                    .key = Engine::Input::Key::Esc,
                    .press_state = Engine::Input::PressState::Down
                },
            },
            .mouse_bindings = {
                Engine::Input::MouseKeyBinding{
                    .name = "confirm",
                    .button = Engine::Input::MouseButton::Left,
                    .press_state = Engine::Input::PressState::Up
                }
            },
        };
        m_engine.RegisterInputMap(pause_input_map);
    }

    GameplayManager::~GameplayManager() = default;

    void GameplayManager::Initialize() const {
        m_engine.RegisterScene("MainMenu",
                               [](const Engine::SceneManagement::SceneArgs& args) {
                                   const auto& mesh_handler = std::any_cast<MeshHandler*>(args.payload);
                                   return std::make_unique<MainMenuScene>(mesh_handler);
                               }
                );

        m_engine.RegisterScene("Game",
                               [](const Engine::SceneManagement::SceneArgs& args) {
                                   const auto game_scene_settings = std::any_cast<GameSceneSettings>(args.payload);
                                   return std::make_unique<GameScene>(game_scene_settings);
                               }
                );

        m_engine.RegisterScene("GameEnd",
                               [](const Engine::SceneManagement::SceneArgs& args) {
                                   const auto game_end_data = std::any_cast<GameEndShowData>(args.payload);
                                   return std::make_unique<GameEndScene>(game_end_data);
                               }
                );

        m_engine.SetInitialScene("MainMenu", Engine::SceneManagement::SceneArgs{m_mesh_handler.get()});
        // m_engine.SetInitialScene("GameEnd",
        //                          Engine::Input::SceneArgs{
        //                              GameEndShowData{m_mesh_handler.get(), 72.0f},
        //
        //                          }
        // );
    }


    void GameplayManager::Shutdown() {
    }
} // namespace
