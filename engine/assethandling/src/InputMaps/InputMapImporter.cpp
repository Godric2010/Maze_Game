#include "InputMapImporter.hpp"

#include <algorithm>
#include <string>
#include <cctype>
#include <iostream>
#include <catch2/generators/catch_generators.hpp>
#include <toml++/toml.hpp>

#include "toml/TomlDocument.hpp"

namespace yarep::AssetHandling::InputMaps
{
    const std::string MapName = "name";
    const std::string KeyBindingsName = "key_bindings";
    const std::string MouseBindingsName = "mouse_bindings";

    const std::string KeyBindingName = "name";
    const std::string MouseBindingName = "name";
    const std::string KeyName = "key";
    const std::string MouseButtonName = "button";
    const std::string PressStateName = "press_state";


    void InputMapImporter::ExtractInputMapFromFileData(input::InputMap& input_map, const std::string& file_content)
    {
        const auto toml_doc = Utilities::Toml::TomlDocument(file_content);
        const auto root_table = toml_doc.GetRootTable();
        input_map.name = root_table.GetRequiredString(MapName);

        ReadKeyBindingsFromFile(input_map, root_table.GetTableList(KeyBindingsName));
        ReadMouseBindingsFromFile(input_map, root_table.GetTableList(MouseBindingsName));
    }

    void InputMapImporter::ReadKeyBindingsFromFile(input::InputMap& input_map,
                                                   const std::vector<Utilities::Toml::TomlTable>& tables)
    {
        for (const auto& table : tables)
        {
            input::KeyBinding key_binding{};
            key_binding.name = table.GetRequiredString(KeyBindingName);
            key_binding.key = table.GetRequiredEnum<input::Key>(KeyName, input::KeyMap);
            key_binding.press_state = table.GetRequiredEnum<input::PressState>(PressStateName, input::PressStateMap);
            input_map.key_bindings.push_back(key_binding);
        }
    }

    void InputMapImporter::ReadMouseBindingsFromFile(input::InputMap& input_map,
                                                     const std::vector<Utilities::Toml::TomlTable>& tables)
    {
        for (const auto& table : tables)
        {
            input::MouseKeyBinding mouse_binding{};
            mouse_binding.name = table.GetRequiredString(MouseBindingName);
            mouse_binding.button = table.GetRequiredEnum<input::MouseButton>(MouseButtonName, input::MouseButtonMap);
            mouse_binding.press_state = table.GetRequiredEnum<input::PressState>(PressStateName, input::PressStateMap);
            input_map.mouse_bindings.push_back(mouse_binding);
        }
    }
}

//namespace
