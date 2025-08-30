#include "EngineController.hpp"

#include "Transform.hpp"
#include "EnvironmentBuilder.hpp"
#include "SystemManager.hpp"

namespace Engine::Core {
    EngineController::EngineController() {
        m_isClosed = false;
        m_isPaused = false;
        m_services = std::make_unique<ServiceLocator>();
    };

    EngineController::~EngineController() = default;

    void EngineController::Initialize(const std::vector<Ecs::SystemMeta> &systems) {
        m_window = Environment::CreateWindow();
        const Environment::WindowConfig config{
            .width = 1920,
            .height = 1080,
            .title = "MazeGame",
            .renderApi = Environment::API::OpenGL,
            .windowMode = Environment::WindowMode::Window
        };
        m_window->Setup(config);

        auto input = Environment::CreateInput(*m_window);
        m_services->RegisterService(std::move(input));

        auto renderController = std::make_unique<Renderer::RenderController>(m_window->GetWindowContext());
        m_services->RegisterService(std::move(renderController));

        m_world = std::make_unique<Ecs::World>();
        m_systemManager = std::make_unique<Ecs::SystemManager>();
        m_systemManager->RegisterSystems(systems, m_services.get());

        m_camera = std::make_unique<Camera>(glm::vec3(0, 0, 3), config.width, config.height, 60, 0.01, 100.0);
    }

    void EngineController::Update() {
        auto lastTime = std::chrono::high_resolution_clock::now();

        while (!m_isClosed) {
            auto now = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration_cast<std::chrono::duration<float> >(now - lastTime).count();
            lastTime = now;

            auto input = PumpInput();
            if (m_isPaused) {
                continue;
            }

            m_systemManager->RunSystems(*m_world, deltaTime);
            UpdateSystems(deltaTime, input);

            m_window->SwapBuffers();
        }
    }

    void EngineController::Shutdown() const {
        m_window->Shutdown();
    }

    Ecs::World &EngineController::GetWorld() const {
        return *m_world;
    }

    Renderer::MeshHandle EngineController::RegisterMesh(const Renderer::MeshAsset &meshAsset) {
        const auto meshHandle = m_services->TryGetService<Renderer::RenderController>()->RegisterMesh(meshAsset);
        return meshHandle;
    }

    Environment::InputSnapshot EngineController::PumpInput() {
        const auto input1 = m_services->TryGetService<Environment::IInput>();
        input1->PrepareFrame();
        input1->PumpInput();
        const auto input = input1->GetInputSnapshot();

        m_isClosed = input->IsClosed;
        // m_isPaused = !input.HasFocus;
        return *input;
    }

    void EngineController::UpdateSystems(const float dt, const Environment::InputSnapshot &snapshot) const {
        constexpr float velocity = 1.0f;
        glm::vec3 newCameraPosition = m_camera->GetPosition();
        if (snapshot.IsKeyHeld(Environment::Key::W)) {
            newCameraPosition.z -= velocity * dt;
        }
        if (snapshot.IsKeyHeld(Environment::Key::S)) {
            newCameraPosition.z += velocity * dt;
        }
        if (snapshot.IsKeyHeld(Environment::Key::A)) {
            newCameraPosition.x -= velocity * dt;
        }
        if (snapshot.IsKeyHeld(Environment::Key::D)) {
            newCameraPosition.x += velocity * dt;
        }
        m_camera->SetPosition(newCameraPosition);

        constexpr float sensitivity = 0.3f;
        const auto mouseDelta = snapshot.GetMouseDelta();
        const float yawDelta = mouseDelta.x * sensitivity;
        const float pitchDelta = mouseDelta.y * sensitivity;
        m_camera->AddYawPitch(yawDelta, -pitchDelta);
    }
} // namespace
