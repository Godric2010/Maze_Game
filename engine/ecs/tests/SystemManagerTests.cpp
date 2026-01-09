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

class TestCommand {
public:
    std::string value;
};

class TestSysA final : public IEngineSystem {
public:
    void Initialize() override {
    }

    void Run(float delta_time) override {
        trace.emplace_back("A");
        SendCommand(TestCommand{});
    }
};

class TestSysB final : public IEngineSystem {
public:
    void Initialize() override {
    }

    void Run(float delta_time) override {
        trace.emplace_back("B");
    }
};

class TestSysC final : public ISystem {
private:
    std::string m_collision_event_name;

public:
    void Initialize() override {
        m_collision_event_name = "None";
    }

    void Run(float delta_time) override {
        trace.emplace_back("C");
        SendCommand(TestCommand{.value = m_collision_event_name});
        m_collision_event_name = "None";
    }

    void OnCollisionEnter(const EntityId& target, const EntityId& other) override {
        m_collision_event_name = "CollisionEnter";
    }

    void OnCollisionExit(const EntityId& target, const EntityId& other) override {
        m_collision_event_name = "CollisionExit";
    }

    void OnTriggerEnter(const EntityId& target, const EntityId& other) override {
        m_collision_event_name = "TriggerEnter";
    }

    void OnTriggerExit(const EntityId& target, const EntityId& other) override {
        m_collision_event_name = "TriggerExit";
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

TEST_CASE("SystemManger::Register - Register different systems and check their execution order") {
    SystemMeta system_a{
        .name = "SystemA",
        .phase = Phase::Input,
        .tags = std::vector<std::string>{"ENGINE"},
        .factory = &MakeA
    };

    SystemMeta system_b{
        .name = "SystemB",
        .phase = Phase::Physics,
        .tags = std::vector<std::string>{"ENGINE"},
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
    auto system_manager = new SystemManager(systems, nullptr, nullptr);
    system_manager->RegisterSystems(world, nullptr);

    system_manager->RunSystems(0.0f);

    const std::vector<std::string> expected = {"A", "B", "C", "D", "E"};
    REQUIRE(trace == expected);
}

TEST_CASE("SystemManager - Push Commands from System to World") {
    const SystemMeta system_a{
        .name = "SystemA",
        .phase = Phase::Input,
        .tags = std::vector<std::string>{"ENGINE"},
        .factory = &MakeA
    };
    const auto world = new World();
    uint8_t callback_called = 0;

    const std::vector<SystemMeta> systems{system_a};
    ISystemManager* system_manager = new SystemManager(systems, nullptr, nullptr);
    system_manager->RegisterSystems(world, nullptr);
    system_manager->RegisterForSystemCommands("TestSubscription",
                                              [&callback_called](const std::vector<std::any>& commands) {
                                                  callback_called++;
                                              }
            );
    system_manager->RunSystems(0.0f);
    REQUIRE(callback_called == 1);
    system_manager->DeregisterForSystemCommands("TestSubscription");
    system_manager->RunSystems(0.0f);
    REQUIRE(callback_called == 1);
    delete system_manager;
}

TEST_CASE("SystemManager - Receive Pysics Events in Systems") {
    const SystemMeta system_c{
        .name = "SystemC",
        .phase = Phase::Update,
        .tags = std::vector<std::string>{"ENGINE"},
        .factory = &MakeC
    };
    const auto world = new World();
    std::string callback_value = "";

    const std::vector<SystemMeta> systems{system_c};
    ISystemManager* system_manager = new SystemManager(systems, nullptr, nullptr);
    system_manager->RegisterSystems(world, nullptr);
    system_manager->RegisterForSystemCommands("TestSubscription",
                                              [&callback_value](const std::vector<std::any>& commands) {
                                                  if (commands.empty()) {
                                                      callback_value = "No_Commands";
                                                      return;
                                                  }
                                                  if (commands.size() > 1) {
                                                      callback_value = "Too_many_commands";
                                                      return;
                                                  }
                                                  auto test_command = any_cast<TestCommand>(commands[0]);
                                                  callback_value = test_command.value;
                                              }
            );
    world->GetPhysicsEventBuffer()->EnqueueEvent(PhysicsEvent{PhysicsEventType::OnCollisionEnter, 0, 0});
    world->ApplyEngineEvents();
    system_manager->RunSystems(0.0f);
    REQUIRE(callback_value == "CollisionEnter");

    world->GetPhysicsEventBuffer()->EnqueueEvent(PhysicsEvent{PhysicsEventType::OnCollisionExit, 0, 0});
    world->ApplyEngineEvents();
    system_manager->RunSystems(0.0f);
    REQUIRE(callback_value == "CollisionExit");

    world->GetPhysicsEventBuffer()->EnqueueEvent(PhysicsEvent{PhysicsEventType::OnTriggerEnter, 0, 0});
    world->ApplyEngineEvents();
    system_manager->RunSystems(0.0f);
    REQUIRE(callback_value == "TriggerEnter");

    world->GetPhysicsEventBuffer()->EnqueueEvent(PhysicsEvent{PhysicsEventType::OnTriggerExit, 0, 0});
    world->ApplyEngineEvents();
    system_manager->RunSystems(0.0f);
    REQUIRE(callback_value == "TriggerExit");

    delete system_manager;
}
