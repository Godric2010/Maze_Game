#if __APPLE__
#include <utility>
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch_all.hpp>
#endif

#include <vector>
#include "AssetTypes.hpp"
#include "../src/Mesh/MeshImporter.hpp"

using namespace Engine::AssetHandling::Mesh;

bool are_vectors_equal(const glm::vec3 actual, const glm::vec3 expected)
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

bool are_vectors_equal(const glm::vec2 actual, const glm::vec2 expected)
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

v 0.0 0.0 0.0
v 1.0 0.0 0.0
v 0.0 1.0 0.0

f 1 2 3)";

    std::vector<Engine::AssetHandling::MeshVertex> vertices;
    std::vector<uint32_t> indices;
    MeshImporter::BuildMeshAssetFromObj(obj_content, vertices, indices);

    REQUIRE(vertices.size() == 3);
    REQUIRE(indices.size() == 3);
    REQUIRE(indices == std::vector<uint32_t>({0, 1, 2}));
    REQUIRE(are_vectors_equal(vertices[0].position, glm::vec3(0,0,0)));
    REQUIRE(are_vectors_equal(vertices[1].position, glm::vec3(1,0,0)));
    REQUIRE(are_vectors_equal(vertices[2].position, glm::vec3(0,1,0)));
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

    std::vector<Engine::AssetHandling::MeshVertex> vertices;
    std::vector<uint32_t> indices;
    MeshImporter::BuildMeshAssetFromObj(obj_content, vertices, indices);

    REQUIRE(vertices.size() == 4);
    REQUIRE(indices.size() == 4);
    REQUIRE(indices == std::vector<uint32_t>({0, 1, 2, 3}));
    REQUIRE(are_vectors_equal(vertices[0].position, glm::vec3(0,0,0)));
    REQUIRE(are_vectors_equal(vertices[1].position, glm::vec3(1,0,0)));
    REQUIRE(are_vectors_equal(vertices[2].position, glm::vec3(1,1,0)));
    REQUIRE(are_vectors_equal(vertices[3].position, glm::vec3(0,1,0)));
    REQUIRE(are_vectors_equal(vertices[0].normal, glm::vec3(0,0,1)));
    REQUIRE(are_vectors_equal(vertices[1].normal, glm::vec3(0,0,1)));
    REQUIRE(are_vectors_equal(vertices[2].normal, glm::vec3(0,0,1)));
    REQUIRE(are_vectors_equal(vertices[3].normal, glm::vec3(0,0,1)));
    REQUIRE(are_vectors_equal(vertices[0].uv, glm::vec2(0,0)));
    REQUIRE(are_vectors_equal(vertices[1].uv, glm::vec2(1,0)));
    REQUIRE(are_vectors_equal(vertices[2].uv, glm::vec2(1,1)));
    REQUIRE(are_vectors_equal(vertices[3].uv, glm::vec2(0,1)));
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

    std::vector<Engine::AssetHandling::MeshVertex> vertices;
    std::vector<uint32_t> indices;
    MeshImporter::BuildMeshAssetFromObj(obj_content, vertices, indices);

    REQUIRE(vertices.size() == 4);
    REQUIRE(indices.size() == 4);
    REQUIRE(indices == std::vector<uint32_t>({0, 1, 2, 3}));
    REQUIRE(are_vectors_equal(vertices[0].position, glm::vec3(-1,-1,1)));
    REQUIRE(are_vectors_equal(vertices[1].position, glm::vec3(1,-1,1)));
    REQUIRE(are_vectors_equal(vertices[2].position, glm::vec3(1,1,1)));
    REQUIRE(are_vectors_equal(vertices[3].position, glm::vec3(-1,1,1)));
    REQUIRE(are_vectors_equal(vertices[0].normal, glm::vec3(0,0,1)));
    REQUIRE(are_vectors_equal(vertices[1].normal, glm::vec3(0,0,1)));
    REQUIRE(are_vectors_equal(vertices[2].normal, glm::vec3(0,0,1)));
    REQUIRE(are_vectors_equal(vertices[3].normal, glm::vec3(0,0,1)));
    REQUIRE(are_vectors_equal(vertices[0].uv, glm::vec2(0,0)));
    REQUIRE(are_vectors_equal(vertices[1].uv, glm::vec2(1,0)));
    REQUIRE(are_vectors_equal(vertices[2].uv, glm::vec2(1,1)));
    REQUIRE(are_vectors_equal(vertices[3].uv, glm::vec2(0,1)));
}
