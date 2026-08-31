#include "InputMapImporter.hpp"

#include <algorithm>
#include <string>
#include <cctype>
#include <iostream>
#include <catch2/generators/catch_generators.hpp>
#include <toml++/toml.hpp>

#include "toml/TomlDocument.hpp"

namespace yarep::asset_handling::input_maps
{
    const std::string map_name = "name";
    const std::string key_bindings_name = "key_bindings";
    const std::string mouse_bindings_name = "mouse_bindings";

    const std::string key_binding_name = "name";
    const std::string mouse_binding_name = "name";
    const std::string key_name = "key";
    const std::string mouse_button_name = "button";
    const std::string press_state_name = "press_state";


    void InputMapImporter::ExtractInputMapFromFileData(input::InputMap& input_map, const std::string& file_content)
    {
        const auto toml_doc = utilities::toml_utils::TomlDocument(file_content);
        const auto root_table = toml_doc.GetRootTable();
        input_map.name = root_table.GetRequiredString(map_name);

        ReadKeyBindingsFromFile(input_map, root_table.GetTableList(key_bindings_name));
        ReadMouseBindingsFromFile(input_map, root_table.GetTableList(mouse_bindings_name));
    }

    void InputMapImporter::ReadKeyBindingsFromFile(input::InputMap& input_map,
                                                   const std::vector<utilities::toml_utils::TomlTable>& tables)
    {
        for (const auto& table : tables)
        {
            input::KeyBinding key_binding{};
            key_binding.name = table.GetRequiredString(key_binding_name);
            key_binding.key = table.GetRequiredEnum<input::Key>(key_name, input::key_map);
            key_binding.press_state = table.GetRequiredEnum<input::PressState>(press_state_name, input::press_state_map);
            input_map.key_bindings.push_back(key_binding);
        }
    }

    void InputMapImporter::ReadMouseBindingsFromFile(input::InputMap& input_map,
                                                     const std::vector<utilities::toml_utils::TomlTable>& tables)
    {
        for (const auto& table : tables)
        {
            input::MouseKeyBinding mouse_binding{};
            mouse_binding.name = table.GetRequiredString(mouse_binding_name);
            mouse_binding.button = table.GetRequiredEnum<input::MouseButton>(mouse_button_name, input::mouse_button_map);
            mouse_binding.press_state = table.GetRequiredEnum<input::PressState>(press_state_name, input::press_state_map);
            input_map.mouse_bindings.push_back(mouse_binding);
        }
    }
}

//namespace
