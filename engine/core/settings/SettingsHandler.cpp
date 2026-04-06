#include "SettingsHandler.hpp"

#include <spdlog/spdlog.h>
#include "toml/TomlDocument.hpp"

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
            return ReadSettingsFromToml(loaded_settings.value);
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
        toml_str += "vsync = " + std::string(render_settings.vsync ? "true\n" : "false\n");
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

    EngineSettings SettingsHandler::ReadSettingsFromToml(const std::string& toml_str)
    {
        const auto toml_doc = Utilities::Toml::TomlDocument(toml_str);
        EngineSettings settings{};
        settings.window = ReadWindowSettingsFromToml(toml_doc.GetRequiredTable("Window"));
        settings.render = ReadRenderSettingsFromToml(toml_doc.GetRequiredTable("Renderer"));
        return settings;
    }

    WindowSettings SettingsHandler::ReadWindowSettingsFromToml(Utilities::Toml::TomlTable table)
    {
        WindowSettings settings{};
        settings.width = table.GetOptionalInt("width").value_or(settings.width);
        settings.height = table.GetOptionalInt("height").value_or(settings.height);
        settings.title = table.GetOptionalString("title").value_or(settings.title);
        settings.mode = table.GetOptionalEnum<WindowMode>("mode", WindowModeMap).value_or(settings.mode);
        return settings;
    }

    RenderSettings SettingsHandler::ReadRenderSettingsFromToml(Utilities::Toml::TomlTable table)
    {
        RenderSettings settings{};
        settings.api = table.GetOptionalEnum<RenderApi>("api", RenderApiMap).value_or(settings.api);
        settings.vsync = table.GetOptionalBool("vsync").value_or(settings.vsync);
        return settings;
    }
} // namespace
