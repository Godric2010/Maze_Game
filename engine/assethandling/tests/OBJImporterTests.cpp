#if __APPLE__
#include <utility>
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch_all.hpp>
#endif

#include <vector>
#include "AssetTypes.hpp"
#include "../src/Mesh/MeshImporter.hpp"

using namespace yarep::asset_handling::mesh;

bool AreVectorsEqual(const glm::vec3 actual, const glm::vec3 expected)
{
    const float x_diff = std::abs(actual.x - expected.x);
    const float y_diff = std::abs(actual.y - expected.y);
    const float z_diff = std::abs(actual.z - expected.z);

    constexpr float tolerance = std::numeric_limits<float>::epsilon();

    if (x_diff > tolerance || y_diff > tolerance || z_diff > tolerance)
    {
        return false;
    }
    return true;
}

bool AreVectorsEqual(const glm::vec2 actual, const glm::vec2 expected)
{
    const float x_diff = std::abs(actual.x - expected.x);
    const float y_diff = std::abs(actual.y - expected.y);

    constexpr float tolerance = std::numeric_limits<float>::epsilon();

    if (x_diff > tolerance || y_diff > tolerance)
    {
        return false;
    }
    return true;
}

TEST_CASE("OBJImporterTests - Analyse simple triangle mesh")
{
    const std::string obj_content = R"(
# Minimal triangle

v 0.0 0.0 0.0 #This is a inline comment, that should be ignored by the parser.
v 1.0 0.0 0.0
v 0.0 1.0 0.0

f 1 2 3)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;
    MeshImporter::BuildMeshAssetFromObj(obj_content, vertices, indices);

    REQUIRE(vertices.size() == 3);
    REQUIRE(indices.size() == 3);
    REQUIRE(indices == std::vector<uint32_t>({0, 1, 2}));
    REQUIRE(AreVectorsEqual(vertices[0].position, glm::vec3(0,0,0)));
    REQUIRE(AreVectorsEqual(vertices[1].position, glm::vec3(1,0,0)));
    REQUIRE(AreVectorsEqual(vertices[2].position, glm::vec3(0,1,0)));
}

TEST_CASE("OBJImporterTests - Analyse minimal quad mesh")
{
    const std::string obj_content = R"(
            # Quad with UVs and a single normal

            v 0.0 0.0 0.0
            v 1.0 0.0 0.0
            v 1.0 1.0 0.0
            v 0.0 1.0 0.0

            vt 0.0 0.0
            vt 1.0 0.0
            vt 1.0 1.0
            vt 0.0 1.0

            vn 0.0 0.0 1.0

            f 1/1/1 2/2/1 3/3/1 4/4/1
)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;
    MeshImporter::BuildMeshAssetFromObj(obj_content, vertices, indices);

    REQUIRE(vertices.size() == 4);
    REQUIRE(indices.size() == 6);
    REQUIRE(indices == std::vector<uint32_t>({0, 1, 2, 0, 2, 3}));
    REQUIRE(AreVectorsEqual(vertices[0].position, glm::vec3(0,0,0)));
    REQUIRE(AreVectorsEqual(vertices[1].position, glm::vec3(1,0,0)));
    REQUIRE(AreVectorsEqual(vertices[2].position, glm::vec3(1,1,0)));
    REQUIRE(AreVectorsEqual(vertices[3].position, glm::vec3(0,1,0)));
    REQUIRE(AreVectorsEqual(vertices[0].normal, glm::vec3(0,0,1)));
    REQUIRE(AreVectorsEqual(vertices[1].normal, glm::vec3(0,0,1)));
    REQUIRE(AreVectorsEqual(vertices[2].normal, glm::vec3(0,0,1)));
    REQUIRE(AreVectorsEqual(vertices[3].normal, glm::vec3(0,0,1)));
    REQUIRE(AreVectorsEqual(vertices[0].uv, glm::vec2(0,1)));
    REQUIRE(AreVectorsEqual(vertices[1].uv, glm::vec2(1,1)));
    REQUIRE(AreVectorsEqual(vertices[2].uv, glm::vec2(1,0)));
    REQUIRE(AreVectorsEqual(vertices[3].uv, glm::vec2(0,0)));
}

TEST_CASE("OBJImporterTests - Analyse cube fragment mesh")
{
    const std::string obj_content = R"(
# Cube fragment with groups, materials and shared positions

mtllib dummy.mtl

o TestObject
g FrontFace
usemtl FrontMat

v -1.0 -1.0  1.0
v  1.0 -1.0  1.0
v  1.0  1.0  1.0
v -1.0  1.0  1.0

vt 0.0 0.0
vt 1.0 0.0
vt 1.0 1.0
vt 0.0 1.0

vn 0.0 0.0 1.0

s 1

f 1/1/1 2/2/1 3/3/1 4/4/1)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;
    MeshImporter::BuildMeshAssetFromObj(obj_content, vertices, indices);

    REQUIRE(vertices.size() == 4);
    REQUIRE(indices.size() == 6);
    REQUIRE(indices == std::vector<uint32_t>({0, 1, 2, 0, 2, 3}));
    REQUIRE(AreVectorsEqual(vertices[0].position, glm::vec3(-1,-1,1)));
    REQUIRE(AreVectorsEqual(vertices[1].position, glm::vec3(1,-1,1)));
    REQUIRE(AreVectorsEqual(vertices[2].position, glm::vec3(1,1,1)));
    REQUIRE(AreVectorsEqual(vertices[3].position, glm::vec3(-1,1,1)));
    REQUIRE(AreVectorsEqual(vertices[0].normal, glm::vec3(0,0,1)));
    REQUIRE(AreVectorsEqual(vertices[1].normal, glm::vec3(0,0,1)));
    REQUIRE(AreVectorsEqual(vertices[2].normal, glm::vec3(0,0,1)));
    REQUIRE(AreVectorsEqual(vertices[3].normal, glm::vec3(0,0,1)));
    REQUIRE(AreVectorsEqual(vertices[0].uv, glm::vec2(0,1)));
    REQUIRE(AreVectorsEqual(vertices[1].uv, glm::vec2(1,1)));
    REQUIRE(AreVectorsEqual(vertices[2].uv, glm::vec2(1,0)));
    REQUIRE(AreVectorsEqual(vertices[3].uv, glm::vec2(0,0)));
}

TEST_CASE("OBJImporterTests - CRLF + Inline comments, no exception")
{
    const std::string obj_crlf_and_inline_comment =
        "v 0 0 0 # comment\r\n"
        "v 1 0 0\r\n"
        "v 0 1 0\r\n"
        "f 1 2 3\r\n";
    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_NOTHROW(MeshImporter::BuildMeshAssetFromObj(obj_crlf_and_inline_comment, vertices, indices));
}

TEST_CASE("OBJImporterTests - Empty obj string")
{
    const std::string obj_empty = "";
    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_THROWS_AS(MeshImporter::BuildMeshAssetFromObj(obj_empty, vertices, indices), std::invalid_argument);
}

TEST_CASE("OBJImporterTests - Invalid vertex line")
{
    const std::string obj_bad_v = R"(
v 0.0 1.0
f 1 2 3
)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_THROWS_AS(MeshImporter::BuildMeshAssetFromObj(obj_bad_v, vertices, indices), std::runtime_error);
}

TEST_CASE("OBJImporterTests - Invalid normal line")
{
    const std::string obj_bad_vn = R"(
vn 0.0 1.0
v 0 0 0
v 1 0 0
v 0 1 0
f 1 2 3
)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_THROWS_AS(MeshImporter::BuildMeshAssetFromObj(obj_bad_vn, vertices, indices), std::runtime_error);
}

TEST_CASE("OBJImporterTests - Invalid UV line")
{
    const std::string obj_bad_vt = R"(
vt 0.5
v 0 0 0
v 1 0 0
v 0 1 0
f 1 2 3
)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_THROWS_AS(MeshImporter::BuildMeshAssetFromObj(obj_bad_vt, vertices, indices), std::runtime_error);
}

TEST_CASE("OBJImporterTests - Invalid face, less than three corners")
{
    const std::string obj_face_too_short = R"(
v 0 0 0
v 1 0 0
f 1 2
)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_THROWS_AS(MeshImporter::BuildMeshAssetFromObj(obj_face_too_short, vertices, indices), std::runtime_error);
}

TEST_CASE("OBJImporterTests - Invalid position index, out of bounds")
{
    const std::string obj_pos_oob = R"(
v 0 0 0
f 2 1 1
)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_THROWS_AS(MeshImporter::BuildMeshAssetFromObj(obj_pos_oob, vertices, indices), std::out_of_range);
}

TEST_CASE("OBJImporterTests - Invalid position index, index is zero")
{
    const std::string obj_pos_zero = R"(
v 0 0 0
v 1 0 0
v 0 1 0
f 0 1 2
)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_THROWS_AS(MeshImporter::BuildMeshAssetFromObj(obj_pos_zero, vertices, indices), std::out_of_range);
}

TEST_CASE("OBJImporterTests - Invalid UV index, out of bounds")
{
    const std::string obj_uv_oob = R"(
v 0 0 0
v 1 0 0
v 0 1 0

vt 0 0

vn 0 0 1

f 1/2/1 2/1/1 3/1/1
)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_THROWS_AS(MeshImporter::BuildMeshAssetFromObj(obj_uv_oob, vertices, indices), std::out_of_range);
}

TEST_CASE("OBJImporterTests - Invalid normal index, out of bounds")
{
    const std::string obj_norm_oob = R"(
v 0 0 0
v 1 0 0
v 0 1 0

vt 0 0
vt 1 0
vt 0 1

vn 0 0 1

f 1/1/2 2/2/1 3/3/1
)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_THROWS_AS(MeshImporter::BuildMeshAssetFromObj(obj_norm_oob, vertices, indices), std::out_of_range);
}

TEST_CASE("OBJImporterTests - Invalid vertex, position is not numeric")
{
    const std::string obj_pos_non_numeric = R"(
v 0 0 0
v 1 0 0
v 0 1 0
f a 2 3
)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_THROWS_AS(MeshImporter::BuildMeshAssetFromObj(obj_pos_non_numeric, vertices, indices), std::runtime_error);
}

TEST_CASE("OBJImporterTests - Invalid UV, uv is not numeric")
{
    const std::string obj_uv_non_numeric = R"(
v 0 0 0
v 1 0 0
v 0 1 0

vt 0 0
vt 1 0
vt 0 1

vn 0 0 1

f 1/x/1 2/2/1 3/3/1
)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_THROWS_AS(MeshImporter::BuildMeshAssetFromObj(obj_uv_non_numeric, vertices, indices), std::runtime_error);
}

TEST_CASE("OBJImporterTests - Invalid normal, normal is not numeric")
{
    const std::string obj_norm_non_numeric = R"(
v 0 0 0
v 1 0 0
v 0 1 0

vt 0 0
vt 1 0
vt 0 1

vn 0 0 1

f 1/1/y 2/2/1 3/3/1
)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_THROWS_AS(MeshImporter::BuildMeshAssetFromObj(obj_norm_non_numeric, vertices, indices), std::runtime_error);
}

TEST_CASE("OBJImporterTests - Invalid object index, out of range")
{
    const std::string obj_index_out_of_range = R"(
v 0 0 0
v 1 0 0
v 0 1 0
f 999999999999999999999999999999 2 3
)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_THROWS_AS(MeshImporter::BuildMeshAssetFromObj(obj_index_out_of_range, vertices, indices),
                      std::runtime_error);
}

TEST_CASE("OBJImporterTests - Invalid index in face, index is negative")
{
    const std::string obj_negative_index = R"(
v 0 0 0
v 1 0 0
v 0 1 0
f -1 2 3
)";

    std::vector<yarep::asset_handling::MeshVertexAsset> vertices;
    std::vector<uint32_t> indices;

    REQUIRE_THROWS_AS(MeshImporter::BuildMeshAssetFromObj(obj_negative_index, vertices, indices), std::runtime_error);
}
