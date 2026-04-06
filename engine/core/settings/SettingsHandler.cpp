#include "SettingsHandler.hpp"

#include <algorithm>
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

    EngineSettings SettingsHandler::ReadSettingsFromToml(const std::string& toml_str)
    {
        toml::table toml_content;
        try
        {
            toml_content = toml::parse(toml_str);
        }
        catch (const toml::parse_error& e)
        {
            spdlog::error(e.what());
            throw;
        }

        EngineSettings settings{};
        settings.window = ReadWindowSettingsFromToml(toml_content);
        settings.render = ReadRenderSettingsFromToml(toml_content);
        return settings;
    }

    WindowSettings SettingsHandler::ReadWindowSettingsFromToml(const toml::table& table)
    {
        WindowSettings settings{};
        toml::node_view<const toml::node> category = table["Window"];
        ReadTomlFieldAsInt(category, "width", settings.width);
        ReadTomlFieldAsInt(category, "height", settings.height);
        ReadTomlFieldAsWindowMode(category, "mode", settings.mode);
        ReadTomlFieldAsString(category, "title", settings.title);
        return settings;
    }

    RenderSettings SettingsHandler::ReadRenderSettingsFromToml(const toml::table& table)
    {
        RenderSettings settings{};
        toml::node_view<const toml::node> category = table["Renderer"];
        ReadTomlFieldAsRenderApi(category, "api", settings.api);
        ReadTomlFieldAsBool(category, "vsync", settings.vsync);
        return settings;
    }

    void SettingsHandler::ReadTomlFieldAsString(toml::node_view<const toml::node> node, const std::string& field_name,
                                                std::string& field_value)
    {
        if (const auto value = node[field_name].value<std::string>())
        {
            field_value = *value;
        }
    }

    void SettingsHandler::ReadTomlFieldAsInt(toml::node_view<const toml::node> node, const std::string& field_name,
                                             int& field_value)
    {
        if (const auto value = node[field_name].value<int>())
        {
            field_value = *value;
        }
    }

    void SettingsHandler::ReadTomlFieldAsBool(toml::node_view<const toml::node> node, const std::string& field_name,
                                              bool& field_value)
    {
        if (const auto value = node[field_name].value<int>())
        {
            field_value = *value > 0;
        }
    }

    void SettingsHandler::ReadTomlFieldAsWindowMode(toml::node_view<const toml::node> node,
                                                    const std::string& field_name, WindowMode& field_value)
    {
        const auto value = node[field_name].value<std::string>();
        if (value.has_value())
        {
            const auto window_mode_str = value.value();
            const auto window_mode_str_normalized = ToLower(window_mode_str);
            for (const auto& [name, mode] : WindowModeMap)
            {
                if (ToLower(name) == window_mode_str_normalized)
                {
                    field_value = mode;
                    return;
                }
            }
        }
    }

    void SettingsHandler::ReadTomlFieldAsRenderApi(toml::node_view<const toml::node> node,
                                                   const std::string& field_name, RenderApi& field_value)
    {
        const auto value = node[field_name].value<std::string>();
        if (value.has_value())
        {
            const auto api_str = value.value();
            const auto api_str_normalized = ToLower(api_str);
            for (const auto& [name, api] : RenderApiMap)
            {
                if (ToLower(name) == api_str_normalized)
                {
                    field_value = api;
                    return;
                }
            }
        }
    }

    std::string SettingsHandler::ToLower(const std::string_view& str)
    {
        std::string result(str);
        std::ranges::transform(result, result.begin(), ::tolower);
        return result;
    }
} // namespace
