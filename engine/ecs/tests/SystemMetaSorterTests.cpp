#if __APPLE__
   #include <catch2/catch_test_macros.hpp>
   #include <utility>
#else
    #include <catch2/catch_all.hpp>
#endif

#include "../src/SystemMetaSorter.hpp"
using namespace Engine::Ecs;

namespace {
    using Engine::Ecs::Phase;
    using Engine::Ecs::SystemMeta;

    SystemMeta MakeMeta(const std::string& name,
                        const Phase phase,
                        std::vector<std::string> dependencies = {},
                        std::vector<std::string> tags = {}) {
        return SystemMeta{
            .name = name,
            .phase = phase,
            .tags = std::move(tags),
            .dependencies = std::move(dependencies),
            .factory = nullptr
        };
    }

    std::vector<std::string> ExtractNames(const std::vector<SystemMeta>& systems) {
        std::vector<std::string> names;
        names.reserve(systems.size());

        for (const auto& system: systems) {
            names.push_back(system.name);
        }

        return names;
    }
}

TEST_CASE("SystemMetaSorter - sorts by phase when no dependencies exist", "[ecs][systems][sorting]") {
    const std::vector input{
        MakeMeta("RenderSystem", Phase::Render),
        MakeMeta("UiSystem", Phase::Ui),
        MakeMeta("InputSystem", Phase::Input),
        MakeMeta("GameplaySystem", Phase::Update)
    };

    const auto sorted = SystemMetaSorter::SortSystemMetasByPhaseAndDependencies(input);
    const auto names = ExtractNames(sorted);

    REQUIRE(names == std::vector<std::string>{
            "InputSystem",
            "UiSystem",
            "GameplaySystem",
            "RenderSystem"
            }
            );
}

TEST_CASE("SystemMetaSorter - sorts systems with same phase deterministically by name", "[ecs][systems][sorting]") {
    const std::vector input{
        MakeMeta("UiTextSystem", Phase::Ui),
        MakeMeta("UiButtonSystem", Phase::Ui),
        MakeMeta("UiImageSystem", Phase::Ui)
    };

    const auto sorted = SystemMetaSorter::SortSystemMetasByPhaseAndDependencies(input);
    const auto names = ExtractNames(sorted);

    REQUIRE(names == std::vector<std::string>{
            "UiButtonSystem",
            "UiImageSystem",
            "UiTextSystem"
            }
            );
}

TEST_CASE("SystemMetaSorter - respects dependencies within same phase", "[ecs][systems][sorting]") {
    const std::vector input{
        MakeMeta("UiButtonSystem", Phase::Ui, {"RectTransformSystem"}),
        MakeMeta("RectTransformSystem", Phase::Ui),
        MakeMeta("UiTextSystem", Phase::Ui, {"RectTransformSystem"})
    };

    const auto sorted = SystemMetaSorter::SortSystemMetasByPhaseAndDependencies(input);
    const auto names = ExtractNames(sorted);

    REQUIRE(names.size() == 3);
    REQUIRE(names[0] == "RectTransformSystem");

    const auto buttonPos = std::ranges::find(names, "UiButtonSystem");
    const auto textPos = std::ranges::find(names, "UiTextSystem");
    const auto rectPos = std::ranges::find(names, "RectTransformSystem");

    REQUIRE(rectPos < buttonPos);
    REQUIRE(rectPos < textPos);
}

TEST_CASE("SystemMetaSorter - respects dependencies across phases", "[ecs][systems][sorting]") {
    const std::vector input{
        MakeMeta("RenderSystem", Phase::Render, {"UiSystem", "CameraSystem"}),
        MakeMeta("CameraSystem", Phase::Update),
        MakeMeta("UiSystem", Phase::Ui)
    };

    const auto sorted = SystemMetaSorter::SortSystemMetasByPhaseAndDependencies(input);
    const auto names = ExtractNames(sorted);

    REQUIRE(names == std::vector<std::string>{
            "UiSystem",
            "CameraSystem",
            "RenderSystem"
            }
            );
}

TEST_CASE("SystemMetaSorter - resolves transitive dependencies", "[ecs][systems][sorting]") {
    const std::vector input{
        MakeMeta("RenderSystem", Phase::Render, {"UiSystem"}),
        MakeMeta("UiSystem", Phase::Ui, {"RectTransformSystem"}),
        MakeMeta("RectTransformSystem", Phase::Ui)
    };

    const auto sorted = SystemMetaSorter::SortSystemMetasByPhaseAndDependencies(input);
    const auto names = ExtractNames(sorted);

    REQUIRE(names == std::vector<std::string>{
            "RectTransformSystem",
            "UiSystem",
            "RenderSystem"
            }
            );
}

TEST_CASE("SystemMetaSorter - prefers earlier phase among currently ready systems", "[ecs][systems][sorting]") {
    const std::vector input{
        MakeMeta("RenderPrepSystem", Phase::Render),
        MakeMeta("UiImageSystem", Phase::Ui),
        MakeMeta("GameplaySystem", Phase::Update)
    };

    const auto sorted = SystemMetaSorter::SortSystemMetasByPhaseAndDependencies(input);
    const auto names = ExtractNames(sorted);

    REQUIRE(names == std::vector<std::string>{
            "UiImageSystem",
            "GameplaySystem",
            "RenderPrepSystem"
            }
            );
}

TEST_CASE("SystemMetaSorter - throws on unknown dependency", "[ecs][systems][sorting]") {
    const std::vector input{
        MakeMeta("RenderSystem", Phase::Render, {"UiSystem"}),
        MakeMeta("CameraSystem", Phase::Update)
    };

    REQUIRE_THROWS_AS(
            SystemMetaSorter::SortSystemMetasByPhaseAndDependencies(input),
            std::runtime_error
            );
}

TEST_CASE("SystemMetaSorter - throws on self dependency", "[ecs][systems][sorting]") {
    const std::vector input{
        MakeMeta("RenderSystem", Phase::Render, {"RenderSystem"})
    };

    REQUIRE_THROWS_AS(
            SystemMetaSorter::SortSystemMetasByPhaseAndDependencies(input),
            std::runtime_error
            );
}

TEST_CASE("SystemMetaSorter - throws on cyclic dependency", "[ecs][systems][sorting]") {
    const std::vector input{
        MakeMeta("UiSystem", Phase::Ui, {"RenderSystem"}),
        MakeMeta("RenderSystem", Phase::Render, {"UiSystem"})
    };

    REQUIRE_THROWS_AS(
            SystemMetaSorter::SortSystemMetasByPhaseAndDependencies(input),
            std::runtime_error
            );
}

TEST_CASE("SystemMetaSorter - throws on duplicate system names", "[ecs][systems][sorting]") {
    const std::vector input{
        MakeMeta("RenderSystem", Phase::Render),
        MakeMeta("RenderSystem", Phase::Ui)
    };

    REQUIRE_THROWS_AS(
            SystemMetaSorter::SortSystemMetasByPhaseAndDependencies(input),
            std::runtime_error
            );
}

TEST_CASE("SystemMetaSorter - throws when dependency violates phase ordering", "[ecs][systems][sorting]") {
    const std::vector input{
        MakeMeta("EarlySystem", Phase::Ui, {"LateSystem"}),
        MakeMeta("LateSystem", Phase::Render)
    };

    REQUIRE_THROWS_AS(
            SystemMetaSorter::SortSystemMetasByPhaseAndDependencies(input),
            std::runtime_error
            );
}
