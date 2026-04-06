#include "toml/TomlDocument.hpp"
#include <algorithm>
#include <spdlog/spdlog.h>

namespace Engine::Utilities::Toml
{
    TomlTable::TomlTable(const toml::node_view<const toml::node> node)
    {
        m_node = node;
    }

    TomlTable::~TomlTable() = default;

    std::string TomlTable::GetRequiredString(const std::string& field_name) const
    {
        const auto value = GetOptionalString(field_name);
        if (value.has_value())
        {
            return *value;
        }
        throw std::runtime_error("Required field in toml table is missing: " + field_name);
    }

    std::optional<std::string> TomlTable::GetOptionalString(const std::string& field_name) const
    {
        const auto field = m_node[field_name];
        if (!field)
        {
            return std::nullopt;
        }
        if (const auto value = field.value<std::string>())
        {
            return *value;
        }
        throw std::runtime_error("TOML field '" + field_name + "'is not a string!");
    }

    int TomlTable::GetRequiredInt(const std::string& field_name) const
    {
        const auto value = GetOptionalInt(field_name);
        if (value.has_value())
        {
            return *value;
        }
        throw std::runtime_error("Required field in toml table is missing: " + field_name);
    }

    std::optional<int> TomlTable::GetOptionalInt(const std::string& field_name) const
    {
        const auto field = m_node[field_name];
        if (!field)
        {
            return std::nullopt;
        }
        if (const auto value = field.value<int>())
        {
            return *value;
        }
        throw std::runtime_error("TOML field '" + field_name + "' is not an integer!");
    }

    bool TomlTable::GetRequiredBool(const std::string& field_name) const
    {
        const auto value = GetOptionalBool(field_name);
        if (value.has_value())
        {
            return *value;
        }
        throw std::runtime_error("Required field in toml table is missing: " + field_name);
    }

    std::optional<bool> TomlTable::GetOptionalBool(const std::string& field_name) const
    {
        const auto field = m_node[field_name];
        if (!field)
        {
            return std::nullopt;
        }
        if (const auto value = field.value<bool>())
        {
            return *value;
        }
        throw std::runtime_error("TOML field '" + field_name + "' is not a boolean!");
    }

    float TomlTable::GetRequiredFloat(const std::string& field_name) const
    {
        const auto value = GetOptionalFloat(field_name);
        if (value.has_value())
        {
            return *value;
        }
        throw std::runtime_error("Required field in toml table is missing: " + field_name);
    }

    std::optional<float> TomlTable::GetOptionalFloat(const std::string& field_name) const
    {
        const auto field = m_node[field_name];
        if (!field)
        {
            return std::nullopt;
        }
        if (const auto value = field.value<float>())
        {
            return *value;
        }
        throw std::runtime_error("TOML field '" + field_name + "' is not a float!");
    }

    glm::vec2 TomlTable::GetRequiredVec2(const std::string& field_name) const
    {
        if (const auto value = GetOptionalVec2(field_name); value.has_value())
        {
            return value.value();
        }
        throw std::runtime_error("Required field in toml table is missing: " + field_name);
    }

    std::optional<glm::vec2> TomlTable::GetOptionalVec2(const std::string& field_name) const
    {
        const auto array = m_node[field_name].as_array();
        if (!array)
        {
            return std::nullopt;
        }

        if (array->size() != 2)
        {
            throw std::runtime_error("Field does not contain two elements as required for a vec2: " + field_name);
        }

        const auto* x_node = array->get(0);
        const auto* y_node = array->get(1);

        if (!x_node || !y_node)
        {
            throw std::runtime_error("Field contains null elements for vec2 conversion: " + field_name);
        }

        const auto x = x_node->value<float>();
        const auto y = y_node->value<float>();

        if (!x || !y)
        {
            throw std::runtime_error("Field contains non-float element for vec2: " + field_name);
        }

        return glm::vec2(x.value(), y.value());
    }

    glm::vec3 TomlTable::GetRequiredVec3(const std::string& field_name) const
    {
        if (const auto value = GetOptionalVec3(field_name); value.has_value())
        {
            return value.value();
        }
        throw std::runtime_error("Required field in toml table is missing: " + field_name);
    }

    std::optional<glm::vec3> TomlTable::GetOptionalVec3(const std::string& field_name) const
    {
        const auto array = m_node[field_name].as_array();
        if (!array)
        {
            return std::nullopt;
        }

        if (array->size() != 3)
        {
            throw std::runtime_error("Field does not contain three elements as required for a vec3: " + field_name);
        }

        const auto* x_node = array->get(0);
        const auto* y_node = array->get(1);
        const auto* z_node = array->get(2);

        if (!x_node || !y_node || !z_node)
        {
            throw std::runtime_error("Field contains null elements for vec3 conversion: " + field_name);
        }

        const auto x = x_node->value<float>();
        const auto y = y_node->value<float>();
        const auto z = z_node->value<float>();

        if (!x || !y || !z)
        {
            throw std::runtime_error("Field contains non-float element for vec3: " + field_name);
        }

        return glm::vec3(x.value(), y.value(), z.value());
    }

    glm::vec4 TomlTable::GetRequiredVec4(const std::string& field_name) const
    {
        if (const auto value = GetOptionalVec4(field_name); value.has_value())
        {
            return value.value();
        }
        throw std::runtime_error("Required field in toml table is missing: " + field_name);
    }

    std::optional<glm::vec4> TomlTable::GetOptionalVec4(const std::string& field_name) const
    {
        const auto array = m_node[field_name].as_array();
        if (!array)
        {
            return std::nullopt;
        }

        if (array->size() != 4)
        {
            throw std::runtime_error("Field does not contain four elements as required for a vec4: " + field_name);
        }

        const auto* x_node = array->get(0);
        const auto* y_node = array->get(1);
        const auto* z_node = array->get(2);
        const auto* w_node = array->get(3);

        if (!x_node || !y_node || !z_node || !w_node)
        {
            throw std::runtime_error("Field contains null elements for vec4 conversion: " + field_name);
        }

        const auto x = x_node->value<float>();
        const auto y = y_node->value<float>();
        const auto z = z_node->value<float>();
        const auto w = w_node->value<float>();

        if (!x || !y || !z || !w)
        {
            throw std::runtime_error("Field contains non-float element for vec4: " + field_name);
        }

        return glm::vec4(x.value(), y.value(), z.value(), w.value());
    }

    TomlTable TomlTable::GetRequiredTable(const std::string& table_name) const
    {
        const auto table = m_node[table_name].as_table();
        if (!table)
        {
            throw std::runtime_error("Field contains null elements for table: " + table_name);
        }
        return TomlTable(toml::node_view(table));
    }

    std::optional<TomlTable> TomlTable::GetOptionalTable(const std::string& table_name) const
    {
        const auto field = m_node[table_name];
        if (!field)
        {
            return std::nullopt;
        }
        const auto table = field.as_table();
        if (!table)
        {
            throw std::runtime_error("Field '" + table_name + "' is not a table");
        }
        return TomlTable(toml::node_view(table));
    }

    std::vector<TomlTable> TomlTable::GetTableList(const std::string& table_name) const
    {
        const auto field = m_node[table_name];
        if (!field)
        {
            return {};
        }
        const auto array = field.as_array();
        if (!array)
        {
            throw std::runtime_error("TOML field '" + table_name + "' is not an array");
        }
        std::vector<TomlTable> result;
        result.reserve(array->size());

        for (size_t i = 0; i < array->size(); ++i)
        {
            const auto* element = array->get(i);
            if (!element)
            {
                throw std::runtime_error("TOML array field '" + table_name + "' contains null element at index: " +
                                         std::to_string(i));
            }
            const auto* table = element->as_table();
            if (!table)
            {
                throw std::runtime_error("TOML array field '" + table_name + "' contains non-table element at index: " +
                                         std::to_string(i));
            }
            result.emplace_back(toml::node_view(table));
        }
        return result;
    }

    TomlDocument::TomlDocument(const std::string& file_content)
    {
        try
        {
            m_root = toml::parse(file_content);
        }
        catch (const toml::parse_error& error)
        {
            spdlog::error(error.what());
            throw;
        }
    }

    TomlTable TomlDocument::GetRootTable() const
    {
        return TomlTable(toml::node_view(m_root));
    }

    TomlTable TomlDocument::GetRequiredTable(const std::string& table_name) const
    {
        const auto table = m_root[table_name].as_table();
        if (!table)
        {
            throw std::runtime_error("Required TOML table is missing: " + table_name);
        }
        return TomlTable(toml::node_view(table));
    }

    std::optional<TomlTable> TomlDocument::GetOptionalTable(const std::string& table_name) const
    {
        const auto field = m_root[table_name];
        if (!field)
        {
            return std::nullopt;
        }
        const auto table = field.as_table();
        if (!table)
        {
            throw std::runtime_error("Field '" + table_name + "' is not a table");
        }
        return TomlTable(toml::node_view(table));
    }
} // namespace
