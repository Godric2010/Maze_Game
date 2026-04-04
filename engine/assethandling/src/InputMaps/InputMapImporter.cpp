#include "InputMapImporter.hpp"

#include <algorithm>
#include <string>
#include <cctype>
#include <iostream>
#include <catch2/generators/catch_generators.hpp>
#include <toml++/toml.hpp>

namespace Engine::AssetHandling::InputMaps
{
    const std::string MapName = "name";
    const std::string KeyBindingsName = "key_bindings";
    const std::string MouseBindingsName = "mouse_bindings";

    const std::string KeyBindingName = "name";
    const std::string MouseBindingName = "name";
    const std::string KeyName = "key";
    const std::string MouseButtonName = "button";
    const std::string PressStateName = "press_state";


    void InputMapImporter::ReadKeyBindingsFromFile(Input::InputMap& input_map, toml::table toml_content)
    {
        if (const auto key_bindings = toml_content[KeyBindingsName].as_array())
        {
            for (auto&& element : *key_bindings)
            {
                if (auto* binding_table = element.as_table())
                {
                    const auto name = ReadFieldAsString(binding_table, KeyBindingName);
                    const auto key = ReadFieldAsKeyboardKey(binding_table);
                    const auto press_state = ReadFieldAsPressState(binding_table);

                    Input::KeyBinding key_binding{
                        .name = name,
                        .key = key,
                        .press_state = press_state
                    };
                    input_map.key_bindings.push_back(key_binding);
                }
            }
        }
    }

    void InputMapImporter::ReadMouseBindingsFromFile(Input::InputMap& input_map, toml::table toml_content)
    {
        if (const auto mouse_bindings = toml_content[MouseBindingsName].as_array())
        {
            for (auto&& element : *mouse_bindings)
            {
                if (auto* binding_table = element.as_table())
                {
                    const auto name = ReadFieldAsString(binding_table, MouseBindingName);
                    const auto button = ReadFieldAsMouseButton(binding_table);
                    const auto press_state = ReadFieldAsPressState(binding_table);

                    Input::MouseKeyBinding mouse_binding{
                        .name = name,
                        .button = button,
                        .press_state = press_state
                    };
                    input_map.mouse_bindings.push_back(mouse_binding);
                }
            }
        }
    }

    void InputMapImporter::ExtractInputMapFromFileData(Input::InputMap& input_map, const std::string& file_content)
    {
        toml::table toml_content;
        try
        {
            toml_content = toml::parse(file_content);
        }
        catch (const toml::parse_error& err)
        {
            std::cerr << "Parse error: " << err.what() << std::endl;
        }

        if (const auto map_name = toml_content[MapName].value_exact<std::string>())
        {
            input_map.name = map_name.value();
        }

        ReadKeyBindingsFromFile(input_map, toml_content);
        ReadMouseBindingsFromFile(input_map, toml_content);
    }

    Input::Key InputMapImporter::ReadFieldAsKeyboardKey(toml::table* table)
    {
        const auto key_str = ReadFieldAsString(table, KeyName);
        const auto key_str_normalized = ToLower(key_str);

        for (const auto& [name, key] : Input::KeyMap)
        {
            if (ToLower(name) == key_str_normalized)
            {
                return key;
            }
        }
        throw std::runtime_error("Key name is not valid: " + key_str);
    }

    Input::MouseButton InputMapImporter::ReadFieldAsMouseButton(toml::table* table)
    {
        const auto mouse_button_str = ReadFieldAsString(table, MouseButtonName);
        const auto mouse_button_str_normalized = ToLower(mouse_button_str);
        for (const auto& [name, mouse_button] : Input::MouseButtonMap)
        {
            if (ToLower(name) == mouse_button_str_normalized)
            {
                return mouse_button;
            }
        }
        throw std::runtime_error("Mouse button name is not valid: " + mouse_button_str);
    }

    Input::PressState InputMapImporter::ReadFieldAsPressState(toml::table* table)
    {
        auto press_state_str = ReadFieldAsString(table, PressStateName);
        auto press_state_str_normalized = ToLower(press_state_str);
        for (const auto& [name, press_state] : Input::PressStateMap)
        {
            if (ToLower(name) == press_state_str_normalized)
            {
                return press_state;
            }
        }
        throw std::runtime_error("PressState name is not valid: " + press_state_str);
    }

    std::string InputMapImporter::ToLower(const std::string_view& str)
    {
        std::string result(str);
        std::ranges::transform(result, result.begin(), ::tolower);
        return result;
    }

    std::string InputMapImporter::ReadFieldAsString(toml::table* binding_table,
                                                    const std::string& binding_name)
    {
        if (const auto field_str = binding_table->get_as<std::string>(binding_name))
        {
            return field_str->get();
        }
        throw std::runtime_error("Field name is not valid: " + binding_name);
    }
} // namespace
