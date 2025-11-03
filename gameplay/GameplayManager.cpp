#include "GameplayManager.hpp"

#include <iostream>

#include "Collider.hpp"
#include "Mesh.hpp"
#include "MotionIntent.hpp"
#include "commands/PauseCommand.hpp"
#include "commands/QuitCommand.hpp"
#include "commands/ui/ButtonClickedCommand.hpp"
#include "components/Camera.hpp"
#include "components/Inventory.hpp"
#include "components/Transform.hpp"
#include "ui/Button.hpp"
#include "ui/Image.hpp"
#include "ui/RectTransform.hpp"

namespace Gameplay {
    GameplayManager::GameplayManager(Engine::Core::IEngine &engine) : m_engine(engine) {
        m_camera_entity = Engine::Ecs::INVALID_ENTITY_ID;
        m_mesh_handler = std::make_unique<MeshHandler>(engine);
        m_maze_builder = std::make_unique<Mazegenerator::MazeBuilder>(&engine.GetWorld(),
                                                                      m_mesh_handler->GetFloorMesh(),
                                                                      m_mesh_handler->GetWallMesh(),
                                                                      m_mesh_handler->GetKeyMesh(),
                                                                      true);
        m_engine.RegisterForSystemCommands([this](const std::vector<std::any> &commands) {
            this->EvaluateCommandsFromSystems(commands);
        });

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
        m_maze_builder->BuildMaze(5, 5, 1337);
        CreateCamera(m_maze_builder->GetMazeStartPosition());
        CreateUi();
        m_engine.EnableInputMap("PlayerInputMap");
    }

    void GameplayManager::Shutdown() {
    }

    void GameplayManager::CreateCamera(glm::vec3 start_pos) {
        m_camera_entity = m_engine.GetWorld().CreateEntity("Player");
        constexpr auto camera_component = Engine::Core::Components::Camera{
            .Width = 1920, // TODO: Use direct data from config later
            .Height = 1080,
            .FieldOfView = 60,
            .AspectRatio = 1920.0f / 1080.0f,
            .NearClip = 0.01f,
            .FarClip = 1000.0f,

        };
        m_engine.GetWorld().AddComponent<Engine::Core::Components::Camera>(m_camera_entity, camera_component);

        const auto camera_transform = Engine::Core::Components::Transform(start_pos,
                                                                          glm::vec3(-10.f, 180.0f, 0.0f));
        m_engine.GetWorld().AddComponent(m_camera_entity, camera_transform);
        const auto camera_motion_intent = Engine::Core::Components::MotionIntent();
        m_engine.GetWorld().AddComponent(m_camera_entity, camera_motion_intent);

        constexpr auto camera_collider = Engine::Core::Components::SphereCollider{
            .is_static = false, .radius = 0.1f
        };
        m_engine.GetWorld().AddComponent(m_camera_entity, camera_collider);

        constexpr auto inventory = Components::Inventory();
        m_engine.GetWorld().AddComponent(m_camera_entity, inventory);
    }

    void GameplayManager::CreateUi() const {
        const auto key_indicator = m_engine.GetWorld().CreateEntity("KeyIndicator");

        constexpr glm::vec2 size = {100, 100};
        constexpr glm::vec2 position = {1920 - size.x - 50, 1080 - size.y - 50};
        const auto transform = Engine::Core::Components::UI::RectTransform(position, size);
        m_engine.GetWorld().AddComponent(key_indicator, transform);

        constexpr auto image = Engine::Core::Components::UI::Image{.color = {1, 0, 0, 0.5}};
        m_engine.GetWorld().AddComponent(key_indicator, image);
    }

    glm::vec3 GameplayManager::ConvertDirection(const Mazegenerator::Direction &direction) {
        switch (direction) {
            case Mazegenerator::Direction::Back:
                return {0.0f, 0.0f, -1.0f};
            case Mazegenerator::Direction::Front:
                return {0.0f, 0.0f, 1.0f};
            case Mazegenerator::Direction::Left:
                return {-1.0f, 0.0f, 0.0f};
            case Mazegenerator::Direction::Right:
                return {1.0f, 0.0f, 0.0f};
            default:
                return {0.0f, 0.0f, 0.0f};
        }
    }

    void GameplayManager::EvaluateCommandsFromSystems(const std::vector<std::any> &commands) {
        for (const auto &command: commands) {
            if (command.type() == typeid(Commands::PauseCommand)) {
                auto pause_command = std::any_cast<Commands::PauseCommand>(command);
                std::cout << "Enable Pause: " << (pause_command.IsPaused() ? "true" : "false") << std::endl;
                pause_command.IsPaused() ? PauseGame() : ResumeGame();
                continue;
            }
            if (command.type() == typeid(Commands::QuitCommand)) {
                m_engine.StopExecution();
                continue;
            }
            if (command.type() == typeid(Engine::Core::Commands::UI::ButtonClickedCommand)) {
                auto button_clicked = std::any_cast<Engine::Core::Commands::UI::ButtonClickedCommand>(command);
                const auto button_id = button_clicked.GetButtonId();
                if (button_id == 1) {
                    ResumeGame();
                } else if (button_id == 2) {
                    m_engine.StopExecution();
                }
            }
        }
    }

    void GameplayManager::PauseGame() const {
        const auto pause_entity = m_engine.GetWorld().CreateEntity("PauseBackground");
        m_engine.GetWorld().AddComponent(pause_entity,
                                         Engine::Core::Components::UI::RectTransform(
                                             glm::vec2((1920 - 1920 * 0.8) / 2, (1080 - 1080 * 0.8) / 2),
                                             glm::vec2(1920 * 0.8, 1080 * 0.8), 0));
        m_engine.GetWorld().AddComponent(pause_entity,
                                         Engine::Core::Components::UI::Image{.color = {0.3, 0.3, 0.3, 0.8}});

        const auto resume_button_entity = m_engine.GetWorld().CreateEntity("ResumeButton");
        constexpr auto pos = glm::vec2((1920 - 1920 * 0.8) / 2 + 100, (1080 - 1080 * 0.8) / 2 + 100);
        m_engine.GetWorld().AddComponent(resume_button_entity,
                                         Engine::Core::Components::UI::RectTransform(pos, glm::vec2(200, 100), 1));

        auto resume_button = Engine::Core::Components::UI::Button();
        resume_button.button_id = 1;
        resume_button.enabled = true;
        resume_button.default_color = {0.0f, 0.7f, 0.0f, 1.0f};
        resume_button.highlight_color = {0.0f, 1.0f, 0.0f, 1.0f};
        resume_button.click_color = {1.0f, 1.0f, 1.0f, 1.0f};
        resume_button.disabled_color = {0.1f, 0.1f, 0.1f, 0.3f};
        m_engine.GetWorld().AddComponent(resume_button_entity, resume_button);

        const auto quit_button_entity = m_engine.GetWorld().CreateEntity("QuitButton");
        constexpr auto quit_pos = pos + glm::vec2(0, 200);
        m_engine.GetWorld().AddComponent(quit_button_entity,
                                         Engine::Core::Components::UI::RectTransform(quit_pos, glm::vec2(200, 100), 1));
        auto quit_button = Engine::Core::Components::UI::Button();
        quit_button.button_id = 2;
        quit_button.enabled = true;
        quit_button.default_color = {0.7f, 0.0f, 0.0f, 1.0f};
        quit_button.highlight_color = {0.1f, 0.0f, 0.0f, 1.0f};
        quit_button.click_color = {1.0f, 1.0f, 1.0f, 1.0f};
        quit_button.disabled_color = {0.1f, 0.1f, 0.1f, 0.3f};
        m_engine.GetWorld().AddComponent(quit_button_entity, quit_button);
        m_engine.DisableInputMap("PlayerInputMap");
        m_engine.EnableInputMap("UIInputMap");
        m_engine.EnableInputMap("PauseInputMap");
    }

    void GameplayManager::ResumeGame() const {
        m_engine.DisableInputMap("UIInputMap");
        m_engine.DisableInputMap("PauseInputMap");
        m_engine.EnableInputMap("PlayerInputMap");

        const auto resume_button = m_engine.GetWorld().GetEntityByName("ResumeButton");
        m_engine.GetWorld().DestroyEntity(resume_button);
        const auto quit_button = m_engine.GetWorld().GetEntityByName("QuitButton");
        m_engine.GetWorld().DestroyEntity(quit_button);

        const auto pause_entity = m_engine.GetWorld().GetEntityByName("PauseBackground");
        m_engine.GetWorld().DestroyEntity(pause_entity);
    }
} // namespace
