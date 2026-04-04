//
// Created by sebastian on 03.04.26.
//

#pragma once
#include <toml++/impl/node_view.hpp>
#include <toml++/impl/value.hpp>

#include "Input/InputTypes.hpp"


namespace Engine::AssetHandling::InputMaps
{
    class InputMapImporter
    {
        public:
            static std::string ReadFieldAsString(toml::table* binding_table, const std::string& binding_name);
            static void ReadKeyBindingsFromFile(Input::InputMap& input_map, toml::table toml_content);
            static void ReadMouseBindingsFromFile(Input::InputMap& input_map, toml::table toml_content);
            static void ExtractInputMapFromFileData(Input::InputMap& input_map, const std::string& file_content);

        private:
            static Input::Key ReadFieldAsKeyboardKey(toml::table* table);
            static Input::MouseButton ReadFieldAsMouseButton(toml::table* table);
            static Input::PressState ReadFieldAsPressState(toml::table* table);
            static std::string ToLower(const std::string_view& str);
    };
}
