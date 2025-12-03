#include "DebugConsole.hpp"

#include <ranges>

namespace Engine::Debug {
    DebugConsole::DebugConsole(Text::TextController* text_controller, Renderer::RenderController* render_controller,
                               const Environment::WindowContext& context,
                               const uint32_t column_width) : m_font_handle(0),
                                                              m_texture_handle(0) {
        m_text_controller = text_controller;
        m_render_controller = render_controller;
        m_column_width = static_cast<float>(column_width);
        m_window_width = static_cast<float>(context.width);
        m_window_height = static_cast<float>(context.height);

        auto [font_handle, _] = m_text_controller->LoadFont("Arial.ttf", m_font_size);
        m_font_handle = font_handle;

        const auto [width, height, pixels] = m_text_controller->GetTextureDescription(font_handle);
        Renderer::TextureAsset texture_asset{};
        texture_asset.width = static_cast<float>(width);
        texture_asset.height = static_cast<float>(height);
        texture_asset.pixels = pixels;

        m_texture_handle = m_render_controller->RegisterTexture(texture_asset);
    }

    DebugConsole::~DebugConsole() {
        m_text_controller = nullptr;
        m_render_controller = nullptr;
        m_column_width = 0;
        m_window_width = 0;
        m_window_height = 0;
        m_font_handle = 0;
        m_texture_handle = 0;
    }

    void DebugConsole::PushValue(const std::string& label, const size_t value) {
        if (m_label_id_map.contains(label)) {
            const auto id = m_label_id_map[label];
            UpdateTextElements(id, std::to_string(value));
            return;
        }

        m_current_label_id++;
        auto text_element = CreateTextElement(label, std::to_string(value));
        m_label_id_map.emplace(label, m_current_label_id);
        m_text_elements.emplace(m_current_label_id, text_element);
    }

    void DebugConsole::PushToFrame() {
        std::vector<Renderer::UiDrawAsset> draw_assets;
        for (const auto& [id, text]: m_text_elements) {
            const uint8_t row = id;
            auto label_asset = CreateUiDrawAsset(0, row, text.label_mesh);
            auto content_asset = CreateUiDrawAsset(1, row, text.content_mesh);
            draw_assets.push_back(label_asset);
            draw_assets.push_back(content_asset);
        }

        m_render_controller->SubmitDebugInfos(draw_assets);
    }

    void DebugConsole::UpdateTextElements(const uint8_t id, const std::string& content) {
        m_text_elements[id].content = content;
        m_text_elements[id].content_mesh = CreateTextMeshElement(content);
    }

    TextElement DebugConsole::CreateTextElement(const std::string& label,
                                                const std::string& content) const {
        TextElement text_element{};
        text_element.label = label;
        text_element.content = content;

        text_element.label_mesh = CreateTextMeshElement(label);
        text_element.content_mesh = CreateTextMeshElement(content);

        return text_element;
    }

    TextMeshElement DebugConsole::CreateTextMeshElement(const std::string& text) const {
        const Text::TextMesh text_mesh = m_text_controller->BuildTextMesh(
                m_font_handle,
                text,
                Text::TextAlignment::Left
                );

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

        const auto mesh = m_render_controller->RegisterMesh(mesh_asset);

        TextMeshElement text_mesh_element{};
        text_mesh_element.mesh_handle = mesh;
        text_mesh_element.width = text_mesh.dimensions_width;
        text_mesh_element.height = text_mesh.dimensions_height;
        return text_mesh_element;
    }

    Renderer::UiDrawAsset DebugConsole::CreateUiDrawAsset(const uint8_t col, const uint8_t row,
                                                          const TextMeshElement text_mesh_element) const {
        constexpr float height_offset = 30;
        constexpr float row_offset = 20;
        constexpr uint8_t max_columns = 2;

        const float pos_x = m_window_width - static_cast<float>(max_columns - col) * m_column_width;
        const float pos_y = height_offset + static_cast<float>(row) * row_offset;

        auto model_mat = glm::mat4(1.0f);
        model_mat = glm::translate(model_mat, glm::vec3(pos_x, pos_y, 0.0f));
        model_mat = glm::scale(model_mat, glm::vec3(text_mesh_element.width, text_mesh_element.height, 1.0f));

        const Renderer::UiDrawAsset draw_asset{
            .model = model_mat,
            .mesh = text_mesh_element.mesh_handle,
            .texture = m_texture_handle,
            .layer = sizeof(uint8_t) - 1,
            .color = glm::vec4(1, 0, 1, 1.0),
        };

        return draw_asset;
    }
} // namespace
