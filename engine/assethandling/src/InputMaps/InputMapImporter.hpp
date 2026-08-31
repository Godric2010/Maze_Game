//
// Created by sebastian on 03.04.26.
//

#pragma once
#include "Input/InputTypes.hpp"
#include "toml/TomlDocument.hpp"


namespace yarep::asset_handling::input_maps
{
    class InputMapImporter
    {
        public:
            static void ExtractInputMapFromFileData(input::InputMap& input_map, const std::string& file_content);

        private:
            static void ReadKeyBindingsFromFile(input::InputMap& input_map,
                                                const std::vector<utilities::toml_utils::TomlTable>& tables);
            static void ReadMouseBindingsFromFile(input::InputMap& input_map,
                                                  const std::vector<utilities::toml_utils::TomlTable>& tables);
    };
}
