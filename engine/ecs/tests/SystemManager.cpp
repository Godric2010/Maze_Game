#if __APPLE__
#include <catch2/catch_test_macros.hpp>
#include <utility>
#else
#include <catch2/catch.hpp>
#endif

#include "../include/SystemManager.hpp"

using namespace Engine::Ecs;

static std::vector<std::string> trace;

struct TestWorld : World {
};

struct SystemManagerFixture {
    SystemManager systemManager;
    TestWorld world;

    SystemManagerFixture() {
        trace.clear();
    }
};

class TestSysA : public ISystem {
public:
    void Run(World &world, float deltaTime) override {
        trace.emplace_back("A");
    }
};

class TestSysB : public ISystem {
public:
    void Run(World &world, float deltaTime) override {
        trace.emplace_back("B");
    }
};

class TestSysC : public ISystem {
public:
    void Run(World &world, float deltaTime) override {
        trace.emplace_back("C");
    }
};

class TestSysD : public ISystem {
public:
    void Run(World &world, float deltaTime) override {
        trace.emplace_back("D");
    }
};

class TestSysE : public ISystem {
public:
    void Run(World &world, float deltaTime) override {
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
    SystemMeta systemA{
        .name = "SystemA",
        .phase = Phase::Input,
        .tags = std::vector<std::string>(),
        .factory = &MakeA
    };

    SystemMeta systemB{
        .name = "SystemB",
        .phase = Phase::Physics,
        .tags = std::vector<std::string>(),
        .factory = &MakeB
    };

    SystemMeta systemC{
        .name = "SystemC",
        .phase = Phase::Update,
        .tags = std::vector<std::string>(),
        .factory = &MakeC
    };
    SystemMeta systemD{
        .name = "SystemD",
        .phase = Phase::LateUpdate,
        .tags = std::vector<std::string>(),
        .factory = &MakeD
    };
    SystemMeta systemE{
        .name = "SystemE",
        .phase = Phase::Render,
        .tags = std::vector<std::string>(),
        .factory = &MakeE
    };

    // Messing up the order on purpose here
    systemManager.RegisterSystem(systemD);
    systemManager.RegisterSystem(systemE);
    systemManager.RegisterSystem(systemC);
    systemManager.RegisterSystem(systemA);
    systemManager.RegisterSystem(systemB);

    systemManager.RunSystems(world, 0.0f);

    const std::vector<std::string> expected = {"A", "B", "C", "D", "E"};
    REQUIRE(trace == expected);
}
