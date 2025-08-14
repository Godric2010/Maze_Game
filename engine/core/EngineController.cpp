#include "EngineController.hpp"

#include "Transform.hpp"
#include "WindowBuilder.hpp"

namespace Engine::Core {
    EngineController::EngineController() {
    }

    EngineController::~EngineController() = default;

    void EngineController::Initialize() {
        m_window = Window::CreateWindow();
        const Window::WindowConfig config{
            .width = 800,
            .height = 600,
            .title = "MazeGame",
            .renderApi = Window::API::OpenGL,
            .windowMode = Window::WindowMode::Window
        };
        m_window->Setup(config);

        m_rendererController = std::make_unique<Renderer::RenderController>(m_window->GetWindowContext());
        m_camera = std::make_unique<Camera>(glm::vec3(0, 0, 3), config.width, config.height, 60, 0.01, 100.0);

        auto quad_mesh = Renderer::MeshAsset{};
        quad_mesh.vertices = std::vector<glm::vec3>();
        quad_mesh.vertices.emplace_back(-0.5, -0.5, 0.0);
        quad_mesh.vertices.emplace_back(0.5, -0.5, 0.0);
        quad_mesh.vertices.emplace_back(0.5, 0.5, 0.0);
        quad_mesh.vertices.emplace_back(-0.5, 0.5, 0.0);
        quad_mesh.indices = std::vector<unsigned int>();
        quad_mesh.indices.push_back(0);
        quad_mesh.indices.push_back(1);
        quad_mesh.indices.push_back(2);
        quad_mesh.indices.push_back(2);
        quad_mesh.indices.push_back(3);
        quad_mesh.indices.push_back(0);
        const Renderer::MeshHandle meshHandle = m_rendererController->RegisterMesh(quad_mesh);

        const auto transform = Transform(glm::vec3(0, 0, 0), glm::vec3(10, 30, 0), glm::vec3(1, 1, 1));
        const auto drawAsset = Renderer::DrawAsset{
            .mesh = meshHandle,
            .model = transform.GetTransform(),
            .color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
        };
        m_drawAssets.push_back(drawAsset);

        const auto transform2 = Transform(glm::vec3(0.5, 0, 1), glm::vec3(0, -10, 0), glm::vec3(1, 1, 1));
        const auto drawAsset2 = Renderer::DrawAsset{
            .mesh = meshHandle,
            .model = transform2.GetTransform(),
            .color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
        };
        m_drawAssets.push_back(drawAsset2);
    }

    void EngineController::Update() const {
        while (true) {
            if (!m_window->PollEvents()) {
                return;
            }
            RenderFrame();

            m_window->SwapBuffers();
        }
    }

    void EngineController::Shutdown() {
        m_rendererController.reset();
        m_window->Shutdown();
    }

    void EngineController::RenderFrame() const {
        Renderer::CameraAsset camAsset = {};
        camAsset.view = m_camera->GetViewMatrix();
        camAsset.projection = m_camera->GetProjectionMatrix();
        camAsset.cameraPosition = glm::vec4(m_camera->GetPosition(), 1.0f);

        m_rendererController->BeginFrame(camAsset);
        m_rendererController->SubmitFrame(m_drawAssets);
    }
} // namespace
