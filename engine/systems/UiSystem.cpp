#include "UiSystem.hpp"

#include <TextController.hpp>
#include <ui/Button.hpp>
#include <ui/Text.hpp>
#include "../core/ServiceLocator.hpp"
#include "../core/commands/ui/ButtonClickedCommand.hpp"


namespace Engine::Core::Systems {
    UiSystem::UiSystem() = default;

    UiSystem::~UiSystem() {
        for (const auto& cache_entry: m_font_textures | std::views::values) {
            m_render_controller->UnregisterTexture(cache_entry);
        }
    }

    void UiSystem::Initialize() {
        m_text_controller = ServiceLocator()->GetService<Text::TextController>();
        m_render_controller = ServiceLocator()->GetService<Renderer::RenderController>();
    }

    void UiSystem::Run(float delta_time) {
        HandleTextLabels();

        Input::InputBuffer input = Input()->GetInput();
        if (!input.IsMapActive("UIInputMap")) {
            return;
        }
        HandleButtons(input);
    }

    bool UiSystem::IsMouseOverElement(const glm::vec2 mouse_pos, const Components::UI::RectTransform* rect) {
        if (mouse_pos.x > rect->GetGlobalPosition().x &&
            mouse_pos.y > rect->GetGlobalPosition().y &&
            mouse_pos.x < rect->GetGlobalPosition().x + rect->GetGlobalSize().x &&
            mouse_pos.y < rect->GetGlobalPosition().y + rect->GetGlobalSize().y) {
            return true;
        }
        return false;
    }

    void UiSystem::HandleButtons(const Input::InputBuffer& input) const {
        auto buttons_with_entities = EcsWorld()->GetComponentsOfType<Components::UI::Button>();
        for (auto [button, entity]: buttons_with_entities) {
            const auto rect = EcsWorld()->GetComponent<Components::UI::RectTransform>(entity);
            if (!button->enabled) {
                button->m_current_color = button->disabled_color;
                continue;
            }

            button->m_current_color = button->default_color;
            if (IsMouseOverElement(input.mouse_position, rect)) {
                button->m_current_color = button->highlight_color;

                if (input.HasAction("UiButtonDown")) {
                    button->m_current_color = button->click_color;
                } else if (input.HasAction("UiButtonUp")) {
                    const auto command = Commands::UI::ButtonClickedCommand(button->button_id);
                    EcsWorld()->PushCommand(command);
                }
            }
        }
    }

    void UiSystem::HandleTextLabels() {
        auto text_labels = EcsWorld()->GetComponentsOfType<Components::UI::Text>();
        for (const auto [text, entity]: text_labels) {
            if (!text->IsDirty()) {
                continue;
            }

            auto [font_handle, new_atlas_created] = m_text_controller->LoadFont(
                    text->GetFontName(),
                    text->GetFontSize()
                    );
            if (new_atlas_created) {
                if (text->GetTextureHandle().has_value()) {
                    m_render_controller->UnregisterTexture(text->GetTextureHandle().value());
                }
            }

            text->m_font_handle = font_handle;
            text->m_texture_handle = GetOrCreateTextureHandleFromFont(font_handle);


            if (text->GetTextMesh().has_value()) {
                m_render_controller->UnregisterMesh(text->GetTextMesh().value());
            }
            auto text_mesh = m_text_controller->BuildTextMesh(text->GetFontHandle(),
                                                              text->GetText(),
                                                              Text::TextAlignment::Left
                    );
            Renderer::MeshAsset text_mesh_asset{};
            for (const auto& vertex: text_mesh.vertices) {
                Renderer::MeshVertex mesh_vertex{
                    .position = glm::vec3(vertex.x, vertex.y, 0),
                    .uv = glm::vec2(vertex.u, vertex.v),
                };
                text_mesh_asset.vertices.emplace_back(mesh_vertex);
            }


            text_mesh_asset.indices = text_mesh.indices;
            auto mesh_handle = m_render_controller->RegisterMesh(text_mesh_asset);

            text->m_text_mesh = mesh_handle;
            text->m_is_dirty = false;

            const auto rect_transform = EcsWorld()->GetComponent<Components::UI::RectTransform>(entity);
            rect_transform->SetSize(glm::vec2(text_mesh.dimensions_width, text_mesh.dimensions_height));
        }
    }

    Renderer::TextureHandle UiSystem::GetOrCreateTextureHandleFromFont(const Text::FontHandle font_handle) {
        if (m_font_textures.contains(font_handle)) {
            return m_font_textures.at(font_handle);
        }
        const auto [width, height, pixels] = m_text_controller->GetTextureDescription(font_handle);
        Renderer::TextureAsset texture_asset{};
        texture_asset.width = static_cast<float>(width);
        texture_asset.height = static_cast<float>(height);
        texture_asset.pixels = pixels;
        const auto texture_handle = m_render_controller->RegisterTexture(texture_asset);
        m_font_textures[font_handle] = texture_handle;
        return texture_handle;
    }
} // namespace
