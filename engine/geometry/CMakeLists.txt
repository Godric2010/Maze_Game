cmake_minimum_required(VERSION 3.31)

add_library(Geometry STATIC
    include/AABB.hpp
        include/Contains.hpp
        src/Contains.cpp
        include/Sphere.hpp
        include/Ray.hpp
        include/OBB.hpp
        include/RayHit.hpp
        include/Bounds.hpp
        src/Bounds.cpp
        include/Intersects.hpp
        src/Intersects.cpp
        include/Plane.hpp
        include/ClosestPoint.hpp
        src/ClosestPoint.cpp
)

target_link_libraries(Geometry PUBLIC Math)
if (COMMAND enable_coverage)
    message("Geometry- Enable Coverage")
    enable_coverage(Geometry)
endif ()

target_include_directories(Geometry PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if (BUILD_TESTING)
    include(testing)

    file(GLOB TEST_SOURCES
            CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
    )
    add_catch2_tests(
            TARGET GEOMETRY_tests
            PREFIX "geometry."
            LINK Geometry
            SOURCES ${TEST_SOURCES}
    )
endif ()
