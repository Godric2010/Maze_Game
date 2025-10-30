#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch_all.hpp>
#endif

#include "../include/SystemManager.hpp"

using namespace Engine::Ecs;

static std::vector<std::string> trace;

struct TestWorld : World {
};

struct SystemManagerFixture {
    SystemManager system_manager;
    TestWorld world;

    SystemManagerFixture() {
        trace.clear();
    }
};

class TestSysA final : public ISystem {
public:
    void Initialize() override {
    }

    void Run(float delta_time) override {
        trace.emplace_back("A");
    }
};

class TestSysB final : public ISystem {
public:
    void Initialize() override {
    }

    void Run(float delta_time) override {
        trace.emplace_back("B");
    }
};

class TestSysC final : public ISystem {
public:
    void Initialize() override {
    }

    void Run(float delta_time) override {
        trace.emplace_back("C");
    }
};

class TestSysD final : public ISystem {
public:
    void Initialize() override {
    }

    void Run(float delta_time) override {
        trace.emplace_back("D");
    }
};

class TestSysE final : public ISystem {
public:
    void Initialize() override {
    }

    void Run(float delta_time) override {
        trace.emplace_back("E");
    }
};

static std::unique_ptr<ISystem> MakeA() { return std::make_unique<TestSysA>(); }
static std::unique_ptr<ISystem> MakeB() { return std::make_unique<TestSysB>(); }
static std::unique_ptr<ISystem> MakeC() { return std::make_unique<TestSysC>(); }
static std::unique_ptr<ISystem> MakeD() { return std::make_unique<TestSysD>(); }
static std::unique_ptr<ISystem> MakeE() { return std::make_unique<TestSysE>(); }

TEST_CASE_METHOD(SystemManagerFixture,
                 "SystemManger::Register - Register different systems and check their execution order") {
    SystemMeta system_a{
        .name = "SystemA",
        .phase = Phase::Input,
        .tags = std::vector<std::string>(),
        .factory = &MakeA
    };

    SystemMeta system_b{
        .name = "SystemB",
        .phase = Phase::Physics,
        .tags = std::vector<std::string>(),
        .factory = &MakeB
    };

    SystemMeta system_c{
        .name = "SystemC",
        .phase = Phase::Update,
        .tags = std::vector<std::string>(),
        .factory = &MakeC
    };
    SystemMeta system_d{
        .name = "SystemD",
        .phase = Phase::LateUpdate,
        .tags = std::vector<std::string>(),
        .factory = &MakeD
    };
    SystemMeta system_e{
        .name = "SystemE",
        .phase = Phase::Render,
        .tags = std::vector<std::string>(),
        .factory = &MakeE
    };
    auto world = new World();

    // Messing up the order on purpose here
    std::vector<SystemMeta> systems{system_d, system_e, system_c, system_a, system_b};
    system_manager.RegisterSystems(systems, world, nullptr, nullptr);

    system_manager.RunSystems(0.0f);

    const std::vector<std::string> expected = {"A", "B", "C", "D", "E"};
    REQUIRE(trace == expected);
}
