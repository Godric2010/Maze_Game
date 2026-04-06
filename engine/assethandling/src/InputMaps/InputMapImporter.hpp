//
// Created by sebastian on 03.04.26.
//

#pragma once
#include "Input/InputTypes.hpp"
#include "toml/TomlDocument.hpp"


namespace Engine::AssetHandling::InputMaps
{
    class InputMapImporter
    {
        public:
            static void ExtractInputMapFromFileData(Input::InputMap& input_map, const std::string& file_content);

        private:
            static void ReadKeyBindingsFromFile(Input::InputMap& input_map,
                                                const std::vector<Utilities::Toml::TomlTable>& tables);
            static void ReadMouseBindingsFromFile(Input::InputMap& input_map,
                                                  const std::vector<Utilities::Toml::TomlTable>& tables);
    };
}
