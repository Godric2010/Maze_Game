//
// Created by sebastian on 03.04.26.
//

#pragma once
#include <toml++/impl/node_view.hpp>

#include "Input/InputTypes.hpp"


namespace Engine::AssetHandling::InputMaps
{
    class InputMapImporter
    {
        public:
            static void ExtractInputMapFromFileData(Input::InputMap& input_map, const std::string& file_content);

        private:
            static Input::Key ReadFieldAsKeyboardKey(toml::table* table);
            static Input::MouseButton ReadFieldAsMouseButton(toml::table* table);
            static Input::PressState ReadFieldAsPressState(toml::table* table);
            static std::string ToLower(const std::string_view& str);
    };
}
