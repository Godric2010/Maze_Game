#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif
#include <array>
#include <string>
#include <string_view>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "../include/toml/TomlDocument.hpp"

namespace Engine::Utilities::Toml::Tests
{
    namespace
    {
        enum class TestEnum
        {
            First,
            Second,
            Third
        };

        constexpr std::array<std::pair<std::string_view, TestEnum>, 3> TestEnumMap{
            {
                {"first", TestEnum::First},
                {"second", TestEnum::Second},
                {"third", TestEnum::Third}
            }
        };

        const std::string ValidToml = R"(
name = "Maze"
count = 42
enabled = true
ratio = 1.5
position2 = [1.0, 2.0]
position3 = [1.0, 2.0, 3.0]
position4 = [1.0, 2.0, 3.0, 4.0]
enum_value = "Second"

[window]
title = "Main Window"
width = 1280
height = 720

[graphics]
vsync = false

[[items]]
name = "Key"
value = 10

[[items]]
name = "Door"
value = 20
)";

        const std::string WrongTypeToml = R"(
name = 123
count = "forty-two"
enabled = "true"
ratio = "1.5"
position2 = [1.0, "wrong"]
position3 = [1.0, 2.0]
position4 = [1.0, 2.0, 3.0]
enum_value = "unknown"

[window]
title = "Main Window"

graphics = "not_a_table"

items = "not_an_array"
)";

        const std::string InvalidToml = R"(
name = "Maze
count = 42
)";
    }

    TEST_CASE("TomlDocument - parses valid TOML", "[Utilities][Toml]")
    {
        REQUIRE_NOTHROW(TomlDocument{ValidToml});
    }

    TEST_CASE("TomlDocument - throws on invalid TOML", "[Utilities][Toml]")
    {
        REQUIRE_THROWS(TomlDocument{InvalidToml});
    }

    TEST_CASE("TomlDocument - GetRootTable returns accessible root table", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};

        const auto root = document.GetRootTable();

        REQUIRE(root.GetRequiredString("name") == "Maze");
        REQUIRE(root.GetRequiredInt("count") == 42);
        REQUIRE(root.GetRequiredBool("enabled"));
        REQUIRE(root.GetRequiredFloat("ratio") == Catch::Approx(1.5f));
    }

    TEST_CASE("TomlDocument - GetRequiredTable returns nested table", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};

        const auto window = document.GetRequiredTable("window");

        REQUIRE(window.GetRequiredString("title") == "Main Window");
        REQUIRE(window.GetRequiredInt("width") == 1280);
        REQUIRE(window.GetRequiredInt("height") == 720);
    }

    TEST_CASE("TomlDocument - GetRequiredTable throws when table is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};

        REQUIRE_THROWS(document.GetRequiredTable("missing_table"));
    }

    TEST_CASE("TomlDocument - GetOptionalTable returns nullopt when table is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};

        const auto table = document.GetOptionalTable("missing_table");

        REQUIRE_FALSE(table.has_value());
    }

    TEST_CASE("TomlDocument - GetOptionalTable throws when field exists but is not a table", "[Utilities][Toml]")
    {
        const TomlDocument document{WrongTypeToml};

        REQUIRE_THROWS(document.GetRootTable().GetRequiredTable("window").GetOptionalTable("graphics"));
    }

    TEST_CASE("TomlTable - GetRequiredString reads string field", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE(root.GetRequiredString("name") == "Maze");
    }

    TEST_CASE("TomlTable - GetOptionalString returns value when field exists", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        const auto value = root.GetOptionalString("name");

        REQUIRE(value.has_value());
        REQUIRE(value.value() == "Maze");
    }

    TEST_CASE("TomlTable - GetOptionalString returns nullopt when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_FALSE(root.GetOptionalString("missing").has_value());
    }

    TEST_CASE("TomlTable - GetRequiredString throws when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetRequiredString("missing"));
    }

    TEST_CASE("TomlTable - GetOptionalString throws when type is wrong", "[Utilities][Toml]")
    {
        const TomlDocument document{WrongTypeToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetOptionalString("name"));
    }

    TEST_CASE("TomlTable - GetRequiredInt reads int field", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE(root.GetRequiredInt("count") == 42);
    }

    TEST_CASE("TomlTable - GetOptionalInt returns nullopt when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_FALSE(root.GetOptionalInt("missing").has_value());
    }

    TEST_CASE("TomlTable - GetRequiredInt throws when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetRequiredInt("missing"));
    }

    TEST_CASE("TomlTable - GetOptionalInt throws when type is wrong", "[Utilities][Toml]")
    {
        const TomlDocument document{WrongTypeToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetOptionalInt("count"));
    }

    TEST_CASE("TomlTable - GetRequiredBool reads bool field", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE(root.GetRequiredBool("enabled"));
    }

    TEST_CASE("TomlTable - GetOptionalBool returns nullopt when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_FALSE(root.GetOptionalBool("missing").has_value());
    }

    TEST_CASE("TomlTable - GetRequiredBool throws when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetRequiredBool("missing"));
    }

    TEST_CASE("TomlTable - GetOptionalBool throws when type is wrong", "[Utilities][Toml]")
    {
        const TomlDocument document{WrongTypeToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetOptionalBool("enabled"));
    }

    TEST_CASE("TomlTable - GetRequiredFloat reads float field", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE(root.GetRequiredFloat("ratio") == Catch::Approx(1.5f));
    }

    TEST_CASE("TomlTable - GetOptionalFloat returns nullopt when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_FALSE(root.GetOptionalFloat("missing").has_value());
    }

    TEST_CASE("TomlTable - GetRequiredFloat throws when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetRequiredFloat("missing"));
    }

    TEST_CASE("TomlTable - GetOptionalFloat throws when type is wrong", "[Utilities][Toml]")
    {
        const TomlDocument document{WrongTypeToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetOptionalFloat("ratio"));
    }

    TEST_CASE("TomlTable - GetRequiredVec2 reads vec2 field", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        const auto value = root.GetRequiredVec2("position2");

        REQUIRE(value.x == Catch::Approx(1.0f));
        REQUIRE(value.y == Catch::Approx(2.0f));
    }

    TEST_CASE("TomlTable - GetOptionalVec2 returns nullopt when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_FALSE(root.GetOptionalVec2("missing").has_value());
    }

    TEST_CASE("TomlTable - GetRequiredVec2 throws when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetRequiredVec2("missing"));
    }

    TEST_CASE("TomlTable - GetOptionalVec2 throws on invalid vec2 data", "[Utilities][Toml]")
    {
        const TomlDocument document{WrongTypeToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetOptionalVec2("position2"));
    }

    TEST_CASE("TomlTable - GetRequiredVec3 reads vec3 field", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        const auto value = root.GetRequiredVec3("position3");

        REQUIRE(value.x == Catch::Approx(1.0f));
        REQUIRE(value.y == Catch::Approx(2.0f));
        REQUIRE(value.z == Catch::Approx(3.0f));
    }

    TEST_CASE("TomlTable - GetOptionalVec3 returns nullopt when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_FALSE(root.GetOptionalVec3("missing").has_value());
    }

    TEST_CASE("TomlTable - GetRequiredVec3 throws when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetRequiredVec3("missing"));
    }

    TEST_CASE("TomlTable - GetOptionalVec3 throws on invalid vec3 data", "[Utilities][Toml]")
    {
        const TomlDocument document{WrongTypeToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetOptionalVec3("position3"));
    }

    TEST_CASE("TomlTable - GetRequiredVec4 reads vec4 field", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        const auto value = root.GetRequiredVec4("position4");

        REQUIRE(value.x == Catch::Approx(1.0f));
        REQUIRE(value.y == Catch::Approx(2.0f));
        REQUIRE(value.z == Catch::Approx(3.0f));
        REQUIRE(value.w == Catch::Approx(4.0f));
    }

    TEST_CASE("TomlTable - GetOptionalVec4 returns nullopt when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_FALSE(root.GetOptionalVec4("missing").has_value());
    }

    TEST_CASE("TomlTable - GetRequiredVec4 throws when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetRequiredVec4("missing"));
    }

    TEST_CASE("TomlTable - GetOptionalVec4 throws on invalid vec4 data", "[Utilities][Toml]")
    {
        const TomlDocument document{WrongTypeToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetOptionalVec4("position4"));
    }

    TEST_CASE("TomlTable - GetRequiredEnum reads enum value case-insensitive", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        const auto value = root.GetRequiredEnum("enum_value", TestEnumMap);

        REQUIRE(value == TestEnum::Second);
    }

    TEST_CASE("TomlTable - GetOptionalEnum returns nullopt when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        const auto value = root.GetOptionalEnum("missing_enum", TestEnumMap);

        REQUIRE_FALSE(value.has_value());
    }

    TEST_CASE("TomlTable - GetRequiredEnum throws when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetRequiredEnum("missing_enum", TestEnumMap));
    }

    TEST_CASE("TomlTable - GetOptionalEnum throws when enum value is invalid", "[Utilities][Toml]")
    {
        const TomlDocument document{WrongTypeToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetOptionalEnum("enum_value", TestEnumMap));
    }

    TEST_CASE("TomlTable - GetRequiredTable reads nested table", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        const auto window = root.GetRequiredTable("window");

        REQUIRE(window.GetRequiredString("title") == "Main Window");
        REQUIRE(window.GetRequiredInt("width") == 1280);
    }

    TEST_CASE("TomlTable - GetRequiredTable throws when nested table is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetRequiredTable("missing_table"));
    }

    TEST_CASE("TomlTable - GetOptionalTable returns nullopt when nested table is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        const auto table = root.GetOptionalTable("missing_table");

        REQUIRE_FALSE(table.has_value());
    }

    TEST_CASE("TomlTable - GetTableList returns all tables from table array", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        const auto items = root.GetTableList("items");

        REQUIRE(items.size() == 2);
        REQUIRE(items[0].GetRequiredString("name") == "Key");
        REQUIRE(items[0].GetRequiredInt("value") == 10);
        REQUIRE(items[1].GetRequiredString("name") == "Door");
        REQUIRE(items[1].GetRequiredInt("value") == 20);
    }

    TEST_CASE("TomlTable - GetTableList returns empty list when field is missing", "[Utilities][Toml]")
    {
        const TomlDocument document{ValidToml};
        const auto root = document.GetRootTable();

        const auto items = root.GetTableList("missing_items");

        REQUIRE(items.empty());
    }

    TEST_CASE("TomlTable - GetTableList throws when field exists but is not an array", "[Utilities][Toml]")
    {
        const TomlDocument document{WrongTypeToml};
        const auto root = document.GetRootTable();

        const auto tables = root.GetTableList("items");
        REQUIRE(tables.empty());
    }

    TEST_CASE("TomlTable - GetTableList throws when array contains non-table elements", "[Utilities][Toml]")
    {
        const std::string toml = R"(
items = [1, 2, 3]
)";

        const TomlDocument document{toml};
        const auto root = document.GetRootTable();

        REQUIRE_THROWS(root.GetTableList("items"));
    }
}
