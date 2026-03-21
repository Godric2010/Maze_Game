#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch_all.hpp>
#endif

#include <iostream>
#include "AssetTypes.hpp"
#include "../src/Materials/MaterialImporter.hpp"

using namespace Engine::AssetHandling;
using namespace Engine::AssetHandling::Materials;

static void CompareTextureMaterialFileData(const MaterialTextureFileData& actual,
                                           const MaterialTextureFileData& expected)
{
    {
        INFO("The texture names in the material do not match");
        REQUIRE(actual.name == expected.name);
    }
    {
        INFO("The tiling in the texture does not match");
        REQUIRE(actual.tiling.x == expected.tiling.x);
        REQUIRE(actual.tiling.y == expected.tiling.y);
    }
    {
        INFO("The uv coordinates of the texture do not match");
        REQUIRE(actual.uv_scale.x == expected.uv_scale.x);
        REQUIRE(actual.uv_scale.y == expected.uv_scale.y);
    }
}

static void CompareMaterialFileData(const MaterialFileData& actual,
                                    const MaterialFileData& expected)
{
    {
        INFO("The material name is not as expected");
        REQUIRE(actual.name == expected.name);
    }
    {
        INFO("The render state is not as expected");
        REQUIRE(actual.render_state == expected.render_state);
    }
    {
        INFO("The render queue index is not as expected");
        REQUIRE(actual.render_queue_index == expected.render_queue_index);
    }
    {
        INFO("The shader name is not as expected");
        REQUIRE(actual.shader_name == expected.shader_name);
    }
    {
        INFO("The base color is not as expected");
        REQUIRE(actual.base_color.r == expected.base_color.r);
        REQUIRE(actual.base_color.g == expected.base_color.g);
        REQUIRE(actual.base_color.b == expected.base_color.b);
        REQUIRE(actual.base_color.a == expected.base_color.a);
    }
    {
        INFO("The albedo texture data is not as expected");
        CompareTextureMaterialFileData(actual.albedo_texture, expected.albedo_texture);
    }
}

TEST_CASE("MaterialImporter - Material toml is valid")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "opaque"
 render_queue = 100
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0, 1.0])";

    MaterialFileData expected = {
        .name = "wall_material",
        .render_state = RenderState::Opaque,
        .render_queue_index = 100,
        .shader_name = "lit_textured",
        .albedo_texture = {
            .name = "Textures/Walls/wall.png",
            .uv_scale = {1.0, 1.0},
            .tiling = {2.0, 2.0}
        },
        .base_color = {1.0, 1.0, 1.0, 1.0}
    };

    MaterialFileData actual;
    MaterialImporter::ExtractMaterialFileData(actual, toml_str);

    CompareMaterialFileData(actual, expected);
}

TEST_CASE("MaterialImporter - Material toml has some float-int variations")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "opaque"
 render_queue = 0
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [0.0, 0.0]
 tiling = [-1.0, 9999.0]

 [colors]
 base = [0.0, 1.0, -5.0, 42])";

    MaterialFileData expected = {
        .name = "wall_material",
        .render_state = RenderState::Opaque,
        .render_queue_index = 0,
        .shader_name = "lit_textured",
        .albedo_texture = {
            .name = "Textures/Walls/wall.png",
            .uv_scale = {0.0, 0.0},
            .tiling = {-1.0, 9999.0}
        },
        .base_color = {0.0, 1.0, -5.0, 42.0}
    };

    MaterialFileData actual;
    MaterialImporter::ExtractMaterialFileData(actual, toml_str);

    CompareMaterialFileData(actual, expected);
}

TEST_CASE("MaterialImporter - Missing fields in toml: Name")
{
    std::string toml_str = R"(
 [material]
 render_type = "opaque"
 render_queue = 100
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Missing fields in toml: render queue")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material""
 render_type = "opaque"
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Missing fields in toml: render state")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_queue = 100
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Missing fields in toml: Shader")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material""
 render_type = "opaque"
 render_queue = 100

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Missing fields in toml: albedo texture name")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material""
 render_type = "opaque"
 render_queue = 100
 shader = "lit_textured"

 [textures.albedo]
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}


TEST_CASE("MaterialImporter - Wrong type in field: number instead of string")
{
    std::string toml_str = R"(
 [material]
 name = 123
 render_type = "opaque"
 render_queue = 100
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Wrong type in field: array instead of string")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "opaque"
 render_queue = 100
 shader = ["lit_textured"]

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Wrong type in field: string instead of int")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "opaque"
 render_queue = "100"
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Wrong type in field: int instead of render state")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = 10
 render_queue = "100"
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Invalid value: render state")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "non-existing-render_type"
 render_queue = "100"
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Invalid value: color vector is too short")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "opaque"
 render_queue = "100"
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Invalid value: uv vector is too long")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "opaque"
 render_queue = "100"
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Invalid value: uv vector is too short")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "opaque"
 render_queue = "100"
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Invalid value: color vector is string")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "opaque"
 render_queue = "100"
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = ["1.0", "1.0", "1.0", "1.0"])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Invalid value: uv vector is string")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "opaque"
 render_queue = "100"
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = ["1.0", "1.0"]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Invalid value: color vector is mixed type")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "opaque"
 render_queue = "100"
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, "1.0", 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Missing color category")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "opaque"
 render_queue = "100"
 shader = "lit_textured"

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]
)";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Missing albedo texture category")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "opaque"
 render_queue = "100"
 shader = "lit_textured"

 [colors]
 base = [1.0, 1.0, 1.0, 1.0])";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Color is in wrong category")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "opaque"
 render_queue = "100"
 shader = "lit_textured" 
 base = [1.0, "1.0", 1.0, 1.0]

 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]
)";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}

TEST_CASE("MaterialImporter - Additional fields")
{
    std::string toml_str = R"(
 [material]
 name = "wall_material"
 render_type = "opaque"
 render_queue = "100"
 shader = "lit_textured" 


 [textures.albedo]
 file_name = "Textures/Walls/wall.png"
 uv_scale = [1.0, 1.0]
 tiling = [2.0, 2.0]

 [colors]
 base = [1.0, "1.0", 1.0, 1.0]
 foo = "bar"
)";

    MaterialFileData actual;
    REQUIRE_THROWS(MaterialImporter::ExtractMaterialFileData(actual, toml_str));
}
