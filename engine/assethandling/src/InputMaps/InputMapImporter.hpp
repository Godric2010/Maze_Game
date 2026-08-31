//
// Created by sebastian on 03.04.26.
//

#pragma once
#include "Input/InputTypes.hpp"
#include "toml/TomlDocument.hpp"


namespace yarep::AssetHandling::InputMaps
{
    class InputMapImporter
    {
        public:
            static void ExtractInputMapFromFileData(input::InputMap& input_map, const std::string& file_content);

        private:
            static void ReadKeyBindingsFromFile(input::InputMap& input_map,
                                                const std::vector<Utilities::Toml::TomlTable>& tables);
            static void ReadMouseBindingsFromFile(input::InputMap& input_map,
                                                  const std::vector<Utilities::Toml::TomlTable>& tables);
    };
}
