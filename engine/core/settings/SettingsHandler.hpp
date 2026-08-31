//
// Created by sebastian on 06.04.26.
//

#pragma once
#include "IFileManager.hpp"
#include "settings.hpp"
#include <toml++/toml.hpp>

#include "toml/TomlDocument.hpp"


namespace yarep::core::settings
{
    class SettingsHandler
    {
        public:
            static void WriteSettingsToDisk(environment::files::IFileManager* file_manager,
                                            const EngineSettings& settings);
            static EngineSettings ReadSettingsFromDisk(environment::files::IFileManager* file_manager);

        private:
            // Write functions
            static std::string CreateTomlFromSettings(const EngineSettings& settings);
            static void AddWindowSettingsToTomlStr(std::string& toml_str, const WindowSettings& window_settings);
            static void AddRenderSettingsToTomlStr(std::string& toml_str, RenderSettings render_settings);
            static std::string GetNameOfWindowMode(WindowMode window_mode);
            static std::string GetNameOfRenderApi(RenderApi api);

            // Read functions
            static EngineSettings ReadSettingsFromToml(const std::string& toml_str);
            static WindowSettings ReadWindowSettingsFromToml(utilities::toml_utils::TomlTable table);
            static RenderSettings ReadRenderSettingsFromToml(utilities::toml_utils::TomlTable table);
    };
}
