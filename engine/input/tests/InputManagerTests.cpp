#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../src/InputManager.hpp"

using namespace Engine::Input;

class FakeEnvInput : public Engine::Environment::IEnvInput {
    bool m_faulty_snapshot = false;

public:
    explicit FakeEnvInput(const bool faulty_snapshot_enabled = false) {
        m_faulty_snapshot = faulty_snapshot_enabled;
    }

    void PrepareFrame() override {
    }

    void PumpInput() override {
    }

    void ShowMouseCursor(bool visible) override {
    }

    Engine::Environment::AppEventsSnapshot* GetAppEventSnapshot() override {
        const auto app_snapshot = new Engine::Environment::AppEventsSnapshot{
            .is_closed = false,
            .has_focus = true,
        };
        return app_snapshot;
    }

    Engine::Environment::InputSnapshot* GetInputSnapshot() override {
        if (m_faulty_snapshot) {
            return nullptr;
        }

        std::unordered_set keys_down = {Engine::Environment::Key::Space};
        std::unordered_set keys_up = {Engine::Environment::Key::A};
        std::unordered_set keys_held = {Engine::Environment::Key::Space};
        std::unordered_set mouse_buttons_down = {Engine::Environment::MouseButton::Left};
        std::unordered_set mouse_buttons_up = {Engine::Environment::MouseButton::Right};
        std::unordered_set mouse_buttons_held = {Engine::Environment::MouseButton::Left};
        const auto input_snapshot = new Engine::Environment::InputSnapshot(glm::vec2(0.1, 0.3),
                                                                           glm::vec2(400, 658),
                                                                           keys_down,
                                                                           keys_held,
                                                                           keys_up,
                                                                           mouse_buttons_down,
                                                                           mouse_buttons_held,
                                                                           mouse_buttons_up
                );
        return input_snapshot;
    }
};

static InputMap CreateInputMap(const std::string& input_map_name) {
    const auto input_map = InputMap{
        .name = input_map_name,
        .key_bindings = {
            KeyBinding{
                .name = "Space_Down",
                .key = Key::Space,
                .press_state = PressState::Down
            },
            KeyBinding{
                .name = "Enter_Hold",
                .key = Key::Enter,
                .press_state = PressState::Pressed
            }
        },
        .mouse_bindings = {
            MouseKeyBinding{
                .name = "MLB_Down",
                .button = MouseButton::Left,
                .press_state = PressState::Down
            }
        }
    };
    return input_map;
}

TEST_CASE("InputManagerTests - Register, Activate and Update Input Map") {
    auto fake_env_input = std::make_unique<FakeEnvInput>();
    auto input_manager = std::make_unique<InputManager>(std::move(fake_env_input));

    const auto input_map_a = CreateInputMap("InputMap_A");

    input_manager->AddInputMapping(input_map_a);

    input_manager->EnableInputMap("InputMap_A");
    input_manager->SetMouseVisibility(true);
    input_manager->UpdateInput();
    auto input_buffer = input_manager->GetInput();

    REQUIRE(input_buffer.IsMapActive("InputMap_A"));
    REQUIRE(input_buffer.HasAction("Space_Down"));
    REQUIRE(input_buffer.HasAction("MLB_Down"));
    REQUIRE_FALSE(input_buffer.HasAction("Enter_Hold"));
    REQUIRE(input_buffer.mouse_delta == glm::vec2(0.1f, 0.3f));
    REQUIRE(input_buffer.mouse_position == glm::vec2(400, 658));
}

TEST_CASE("InputManagerTests - Register same map name twice") {
    auto fake_env_input = std::make_unique<FakeEnvInput>();
    auto input_manager = std::make_unique<InputManager>(std::move(fake_env_input));

    const auto input_map_a1 = CreateInputMap("InputMap_A");
    const auto input_map_a2 = CreateInputMap("InputMap_A");

    input_manager->AddInputMapping(input_map_a1);
    REQUIRE_THROWS(input_manager->AddInputMapping(input_map_a2));
}

TEST_CASE("InputManagerTests - Activating same map twice") {
    auto fake_env_input = std::make_unique<FakeEnvInput>();
    auto input_manager = std::make_unique<InputManager>(std::move(fake_env_input));

    const auto input_map_a = CreateInputMap("InputMap_A");
    input_manager->AddInputMapping(input_map_a);

    input_manager->EnableInputMap("InputMap_A");
    REQUIRE_NOTHROW(input_manager->EnableInputMap("InputMap_A"));

    input_manager->UpdateInput();
    auto input_buffer = input_manager->GetInput();
    REQUIRE(input_buffer.IsMapActive("InputMap_A"));
    REQUIRE(input_buffer.active_map_names.size() == 1);
}

TEST_CASE("InputManagerTests - Deactivating input map") {
    auto fake_env_input = std::make_unique<FakeEnvInput>();
    auto input_manager = std::make_unique<InputManager>(std::move(fake_env_input));
    const auto input_map_a = CreateInputMap("InputMap_A");
    input_manager->AddInputMapping(input_map_a);
    input_manager->EnableInputMap("InputMap_A");
    input_manager->UpdateInput();
    input_manager->DisableInputMap("InputMap_A");
    input_manager->UpdateInput();

    auto input_buffer = input_manager->GetInput();
    REQUIRE_FALSE(input_buffer.IsMapActive("InputMap_A"));
    REQUIRE(input_buffer.active_map_names.empty());
}

TEST_CASE("InputManagerTests - Enable two maps at the same time") {
    auto fake_env_input = std::make_unique<FakeEnvInput>();
    auto input_manager = std::make_unique<InputManager>(std::move(fake_env_input));
    const auto input_map_a = CreateInputMap("InputMap_A");
    const auto input_map_b = CreateInputMap("InputMap_B");
    input_manager->AddInputMapping(input_map_a);
    input_manager->AddInputMapping(input_map_b);
    input_manager->EnableInputMap("InputMap_A");
    input_manager->EnableInputMap("InputMap_B");
    input_manager->UpdateInput();
    auto input_buffer = input_manager->GetInput();

    REQUIRE(input_buffer.IsMapActive("InputMap_A"));
    REQUIRE(input_buffer.IsMapActive("InputMap_B"));
}

TEST_CASE("InputManagerTests - Disable input map that was not active before") {
    auto fake_env_input = std::make_unique<FakeEnvInput>();
    auto input_manager = std::make_unique<InputManager>(std::move(fake_env_input));
    const auto input_map_a = CreateInputMap("InputMap_A");

    input_manager->AddInputMapping(input_map_a);
    REQUIRE_THROWS(input_manager->DisableInputMap("InputMap_A"));
}

TEST_CASE("InputManagerTests - Faulty Env Input") {
    constexpr bool faulty_env_input = true;
    auto fake_env_input = std::make_unique<FakeEnvInput>(faulty_env_input);
    const auto input_manager = std::make_unique<InputManager>(std::move(fake_env_input));
    const auto input_map_a = CreateInputMap("InputMap_A");
    input_manager->AddInputMapping(input_map_a);
    input_manager->EnableInputMap("InputMap_A");

    REQUIRE_THROWS(input_manager->UpdateInput());
}

TEST_CASE("InputManagerTests - No active input map") {
    auto fake_env_input = std::make_unique<FakeEnvInput>();
    auto input_manager = std::make_unique<InputManager>(std::move(fake_env_input));
    const auto input_map_a = CreateInputMap("InputMap_A");
    input_manager->AddInputMapping(input_map_a);

    input_manager->UpdateInput();
    auto input_buffer = input_manager->GetInput();
    REQUIRE_FALSE(input_buffer.IsMapActive("InputMap_A"));
    REQUIRE(input_buffer.active_map_names.empty());
    REQUIRE(input_buffer.actions.empty());
}

TEST_CASE("InputManagerTests - AppSnapshot") {
    auto fake_env_input = std::make_unique<FakeEnvInput>();
    const auto input_manager = std::make_unique<InputManager>(std::move(fake_env_input));

    const auto app_snapshot = input_manager->GetAppEventSnapshot();
    REQUIRE(app_snapshot->has_focus);
    REQUIRE_FALSE(app_snapshot->is_closed);
}
