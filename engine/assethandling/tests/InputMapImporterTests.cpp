#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

#include "../src/InputMaps/InputMapImporter.hpp"

using namespace Engine::Input;
using namespace Engine::AssetHandling;
using namespace Engine::AssetHandling::InputMaps;

static void CompareInputMaps(const InputMap& actual, const InputMap& expected)
{
    {
        INFO("The name of the input map does not match");
        REQUIRE(actual.name == expected.name);
    }
    {
        INFO("The amount of mapped key bindings does not match.");
        REQUIRE(actual.key_bindings.size() == expected.key_bindings.size());
    }
    {
        INFO("The amount of mapped mouse bindings does match.");
        REQUIRE(actual.mouse_bindings.size() == expected.mouse_bindings.size());
    }

    for (int i = 0; i < expected.mouse_bindings.size(); ++i)
    {
        {
            INFO("The mouse binding name does not match.");
            REQUIRE(actual.mouse_bindings[i].name == expected.mouse_bindings[i].name);
        }
        {
            INFO("The mouse binding button does not match.");
            REQUIRE(actual.mouse_bindings[i].button == expected.mouse_bindings[i].button);
        }
        {
            INFO("The mouse binding press state does not match.");
            REQUIRE(actual.mouse_bindings[i].press_state == expected.mouse_bindings[i].press_state);
        }
    }

    for (int i = 0; i < expected.key_bindings.size(); ++i)
    {
        {
            INFO("The key binding name does not match.");
            REQUIRE(actual.key_bindings[i].name == expected.key_bindings[i].name);
        }
        {
            INFO("The key binding key does not match.");
            REQUIRE(actual.key_bindings[i].key == expected.key_bindings[i].key);
        }
        {
            INFO("The key binding press state does not match.");
            REQUIRE(actual.key_bindings[i].press_state == expected.key_bindings[i].press_state);
        }
    }
}

TEST_CASE("InputMapImporter - InputMap toml is valid")
{
    std::string toml_str = R"(
name = "TestInputMap"
[[key_bindings]]
name = "forward"
key = "W"
press_state = "Pressed"

[[key_bindings]]
name = "backward"
key = "s"
press_state = "Pressed"

[[key_bindings]]
name = "left"
key = "a"
press_state = "UP"

[[key_bindings]]
name = "right"
key = "shiftLeft"
press_state = "down"

[[key_bindings]]
name = "pause"
key = "ESC"
press_state = "DoWn"

[[mouse_bindings]]
name = "MouseLeft"
button = "left"
press_state = "pressed"

[[mouse_bindings]]
name = "MouseRight"
button = "right"
press_state = "Up"

[[mouse_bindings]]
name = "MouseMiddle"
button = "middle"
press_state = "Down"
)";

    InputMap expected = {
        .name = "TestInputMap",
        .key_bindings = std::vector<KeyBinding>{
            KeyBinding{.name = "forward", .key = Key::W, .press_state = PressState::Pressed},
            KeyBinding{.name = "backward", .key = Key::S, .press_state = PressState::Pressed},
            KeyBinding{.name = "left", .key = Key::A, .press_state = PressState::Up},
            KeyBinding{.name = "right", .key = Key::ShiftLeft, .press_state = PressState::Down},
            KeyBinding{.name = "pause", .key = Key::Esc, .press_state = PressState::Down},
        },
        .mouse_bindings = std::vector<MouseKeyBinding>{
            MouseKeyBinding{.name = "MouseLeft", .button = MouseButton::Left, .press_state = PressState::Pressed},
            MouseKeyBinding{.name = "MouseRight", .button = MouseButton::Right, .press_state = PressState::Up},
            MouseKeyBinding{.name = "MouseMiddle", .button = MouseButton::Middle, .press_state = PressState::Down},
        },
    };

    InputMap actual;
    InputMapImporter::ExtractInputMapFromFileData(actual, toml_str);
    CompareInputMaps(actual, expected);
}
