#include "DebugUIDrawer.hpp"

#include <ranges>

#include "../../Build/Debug-with-Coverage/vcpkg_installed/arm64-osx/include/spdlog/spdlog.h"

namespace Engine::Core {
    DebugUiDrawer::DebugUiDrawer(Text::TextController* text_controller, Renderer::RenderController* render_controller,
                                 const Environment::WindowContext& context) {
        m_render_controller = render_controller;
        m_text_controller = text_controller;
        m_window_width = context.width;
        m_window_height = context.height;

        auto [font_handle, _] = m_text_controller->LoadFont("Arial.ttf", m_font_size);
        m_font_handle = font_handle;

        const auto [width, height, pixels] = m_text_controller->GetTextureDescription(font_handle);
        Renderer::TextureAsset texture_asset{};
        texture_asset.width = static_cast<float>(width);
        texture_asset.height = static_cast<float>(height);
        texture_asset.pixels = pixels;

        m_texture_handle = m_render_controller->RegisterTexture(texture_asset);
    }

    DebugUiDrawer::~DebugUiDrawer() {
        m_text_controller = nullptr;
        m_render_controller = nullptr;

        m_font_handle = 0;
        m_texture_handle = 0;
    }

    void DebugUiDrawer::SetDrawData(std::unordered_map<uint8_t, std::string>& data) {
        for (auto& [id, content]: data) {
            UpdateTextElements(id, content);
        }
    }


    std::vector<Renderer::UiDrawAsset> DebugUiDrawer::DrawDebugUi() {
        std::vector<Renderer::UiDrawAsset> draw_assets;
        for (const auto& text_element: m_text_elements | std::views::values) {
            auto model_mat = glm::mat4(1.0f);
            model_mat = glm::translate(model_mat, glm::vec3(m_window_width - 180, 30 + text_element.id * 19, 0.0f));
            model_mat = glm::scale(model_mat, glm::vec3(text_element.text_width, text_element.text_height, 1.0f));

            Renderer::UiDrawAsset draw_asset{
                .model = model_mat,
                .mesh = text_element.mesh_handle,
                .texture = m_texture_handle,
                .layer = sizeof(uint8_t) - 1,
                .color = glm::vec4(1, 0, 1, 1.0),
            };
            draw_assets.push_back(draw_asset);
        }

        return draw_assets;
    }

    void DebugUiDrawer::UpdateTextElements(const uint8_t id, const std::string& content) {
        if (m_text_elements.contains(id)) {
            const auto text_element = m_text_elements[id];
            if (text_element.content == content) {
                return;
            }
            m_render_controller->UnregisterMesh(text_element.mesh_handle);
            const auto debug_element = CreateTextElements(id, content);
            m_text_elements[id] = debug_element;
            return;
        }

        auto debug_element = CreateTextElements(id, content);
        m_text_elements.emplace(id, debug_element);
    }

    DebugElement DebugUiDrawer::CreateTextElements(uint8_t id, const std::string& content) const {
        Text::TextMesh text_mesh = m_text_controller->BuildTextMesh(m_font_handle, content, Text::TextAlignment::Left);

        std::vector<Renderer::MeshVertex> text_vertices;
        for (auto& vertex: text_mesh.vertices) {
            Renderer::MeshVertex mesh_vertex{};
            mesh_vertex.position = glm::vec3(vertex.x, vertex.y, 0);
            mesh_vertex.uv = glm::vec2(vertex.u, vertex.v);
            text_vertices.push_back(mesh_vertex);
        }

        Renderer::MeshAsset mesh_asset;
        mesh_asset.vertices = text_vertices;
        mesh_asset.indices = text_mesh.indices;

        auto mesh = m_render_controller->RegisterMesh(mesh_asset);

        DebugElement element;
        element.id = id;
        element.content = content;
        element.mesh_handle = mesh;
        element.text_width = text_mesh.dimensions_width;
        element.text_height = text_mesh.dimensions_height;
        return element;
    }
} // namespace
