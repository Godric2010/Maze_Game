//
// Created by sebastian on 06.04.26.
//

#pragma once
#include "IFileManager.hpp"
#include "Settings.hpp"
#include <toml++/toml.hpp>


namespace Engine::Core::Settings
{
    class SettingsHandler
    {
        public:
            static void WriteSettingsToDisk(Environment::Files::IFileManager* file_manager,
                                            const EngineSettings& settings);
            static EngineSettings ReadSettingsFromDisk(Environment::Files::IFileManager* file_manager);

        private:
            // Write functions
            static std::string CreateTomlFromSettings(const EngineSettings& settings);
            static void AddWindowSettingsToTomlStr(std::string& toml_str, const WindowSettings& window_settings);
            static void AddRenderSettingsToTomlStr(std::string& toml_str, RenderSettings render_settings);
            static std::string GetNameOfWindowMode(WindowMode window_mode);
            static std::string GetNameOfRenderApi(RenderApi api);

            // Read functions
            static EngineSettings ReadSettingsFromToml(const std::string& toml_str);
            static WindowSettings ReadWindowSettingsFromToml(const toml::table& table);
            static RenderSettings ReadRenderSettingsFromToml(const toml::table& table);

            static void ReadTomlFieldAsString(toml::node_view<const toml::node> node, const std::string& field_name,
                                              std::string& field_value);
            static void ReadTomlFieldAsInt(toml::node_view<const toml::node> node, const std::string& field_name,
                                           int& field_value);
            static void ReadTomlFieldAsBool(toml::node_view<const toml::node> node, const std::string& field_name,
                                            bool& field_value);
            static void ReadTomlFieldAsWindowMode(toml::node_view<const toml::node> node, const std::string& field_name,
                                                  WindowMode& field_value);
            static void ReadTomlFieldAsRenderApi(toml::node_view<const toml::node> node, const std::string& field_name,
                                                 RenderApi& field_value);
            static std::string ToLower(const std::string_view& str);
    };
}
