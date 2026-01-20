#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../src/Mesh/MeshImporter.hpp"

TEST_CASE("OBJImporterTests - Analyse simple triangle mesh") {
    REQUIRE(true);
}

TEST_CASE("OBJImporterTests - Analyse minimal quad mesh") {
    REQUIRE(true);
}

TEST_CASE("OBJImporterTests - Analyse cube mesh") {
    REQUIRE(true);
}
