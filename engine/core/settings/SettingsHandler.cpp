#include "SettingsHandler.hpp"

#include <spdlog/spdlog.h>

namespace Engine::Core::Settings
{
    void SettingsHandler::WriteSettingsToDisk(Environment::Files::IFileManager* file_manager,
                                              const EngineSettings& settings)
    {
        const std::string m_settings_file_name = "settings.toml";
        const std::string m_settings_file_path = "";
        const std::string toml_content = CreateTomlFromSettings(settings);
        file_manager->WriteTextToFile(m_settings_file_path, m_settings_file_name, toml_content);
    }

    EngineSettings SettingsHandler::ReadSettingsFromDisk(Environment::Files::IFileManager* file_manager)
    {
        const auto loaded_settings = file_manager->ReadTextFromFile("settings.toml");
        if (loaded_settings.Ok())
        {
            return EngineSettings{};
        }

        if (loaded_settings.type == Environment::Files::ResultType::FileNotFound)
        {
            spdlog::info("Settings file not found. Creating new settings file with default values.");
        }
        else if (loaded_settings.type == Environment::Files::ResultType::FileError)
        {
            spdlog::warn("Settings file is corrupted. Overriding settings with default state.");
        }

        EngineSettings default_settings{};
        WriteSettingsToDisk(file_manager, default_settings);
        return default_settings;
    }

    std::string SettingsHandler::CreateTomlFromSettings(const EngineSettings& settings)
    {
        std::string toml_str;
        AddWindowSettingsToTomlStr(toml_str, settings.window);
        AddRenderSettingsToTomlStr(toml_str, settings.render);
        return toml_str;
    }


    void SettingsHandler::AddWindowSettingsToTomlStr(std::string& toml_str, const WindowSettings& window_settings)
    {
        toml_str += "[Window]\n";
        toml_str += "width = " + std::to_string(window_settings.width) + "\n";
        toml_str += "height = " + std::to_string(window_settings.height) + "\n";
        toml_str += "mode = \"" + GetNameOfWindowMode(window_settings.mode) + "\"\n";
        toml_str += "title = \"" + window_settings.title + "\"\n";
        // Add new settings here
        toml_str += "\n";
    }

    void SettingsHandler::AddRenderSettingsToTomlStr(std::string& toml_str, const RenderSettings render_settings)
    {
        toml_str += "[Renderer]\n";
        toml_str += "api = \"" + GetNameOfRenderApi(render_settings.api) + "\"\n";
        toml_str += "vsync = " + std::to_string(render_settings.vsync ? 1 : 0) + "\n";
        // Add new settings here
        toml_str += "\n";
    }

    std::string SettingsHandler::GetNameOfWindowMode(const WindowMode window_mode)
    {
        for (const auto& [name, mode] : WindowModeMap)
        {
            if (mode == window_mode)
            {
                return std::string{name};
            }
        }
        throw std::runtime_error("Unknown window mode.");
    }

    std::string SettingsHandler::GetNameOfRenderApi(const RenderApi api)
    {
        for (const auto& [name, render_api] : RenderApiMap)
        {
            if (render_api == api)
            {
                return std::string{name};
            }
        }
        throw std::runtime_error("Unknown render api.");
    }
} // namespace
