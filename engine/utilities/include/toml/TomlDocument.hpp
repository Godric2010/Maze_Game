//
// Created by sebastian on 06.04.26.
//

#pragma once
#include <algorithm>
#include <string>
#include <optional>
#include <toml++/toml.hpp>
#include <glm/glm.hpp>


namespace Engine::Utilities::Toml
{
    class TomlTable
    {
        public:
            explicit TomlTable(toml::node_view<const toml::node> node);
            ~TomlTable();

            std::string GetRequiredString(const std::string& field_name) const;
            std::optional<std::string> GetOptionalString(const std::string& field_name) const;

            int GetRequiredInt(const std::string& field_name) const;
            std::optional<int> GetOptionalInt(const std::string& field_name) const;

            bool GetRequiredBool(const std::string& field_name) const;
            std::optional<bool> GetOptionalBool(const std::string& field_name) const;

            float GetRequiredFloat(const std::string& field_name) const;
            std::optional<float> GetOptionalFloat(const std::string& field_name) const;

            glm::vec2 GetRequiredVec2(const std::string& field_name) const;
            std::optional<glm::vec2> GetOptionalVec2(const std::string& field_name) const;

            glm::vec3 GetRequiredVec3(const std::string& field_name) const;
            std::optional<glm::vec3> GetOptionalVec3(const std::string& field_name) const;

            glm::vec4 GetRequiredVec4(const std::string& field_name) const;
            std::optional<glm::vec4> GetOptionalVec4(const std::string& field_name) const;

            template <typename TEnum, size_t N>
            TEnum GetRequiredEnum(const std::string& field_name,
                                  const std::array<std::pair<std::string_view, TEnum>, N>& mapping) const
            {
                {
                    const auto value_str = GetRequiredString(field_name);
                    const auto value_str_normalized = NormalizeString(value_str);
                    for (const auto& [name, value] : mapping)
                    {
                        const auto name_normalized = NormalizeString(name);
                        if (name_normalized == value_str_normalized)
                        {
                            return value;
                        }
                    }
                    throw std::runtime_error("Required field in toml table has no value called: " + value_str);
                }
            };

            template <typename TEnum, size_t N>
            std::optional<TEnum> GetOptionalEnum(const std::string& field_name,
                                                 const std::array<std::pair<std::string_view, TEnum>, N>& mapping) const
            {
                const auto value_str = GetOptionalString(field_name);
                if (!value_str.has_value())
                {
                    return std::nullopt;
                }
                const auto value_str_normalized = NormalizeString(value_str.value());
                for (const auto& [name, value] : mapping)
                {
                    const auto name_normalized = NormalizeString(name);
                    if (name_normalized == value_str_normalized)
                    {
                        return value;
                    }
                }
                throw std::runtime_error("Required field in toml table has no value called: " + value_str.value());
            };

            TomlTable GetRequiredTable(const std::string& table_name) const;
            std::optional<TomlTable> GetOptionalTable(const std::string& table_name) const;
            std::vector<TomlTable> GetTableList(const std::string& table_name) const;

        private:
            toml::node_view<const toml::node> m_node;

            static inline std::string NormalizeString(const std::string_view value)
            {
                std::string result(value);
                std::ranges::transform(result, result.begin(), [](const unsigned char c)
                {
                    return std::tolower(c);
                });
                return result;
            }
    };

    class TomlDocument
    {
        public:
            explicit TomlDocument(const std::string& file_content);

            [[nodiscard]] TomlTable GetRootTable() const;
            [[nodiscard]] TomlTable GetRequiredTable(const std::string& table_name) const;
            [[nodiscard]] std::optional<TomlTable> GetOptionalTable(const std::string& table_name) const;

        private:
            toml::table m_root;
    };
}
