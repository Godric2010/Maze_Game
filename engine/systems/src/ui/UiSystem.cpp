#include "UiSystem.hpp"

#include <TextController.hpp>
#include <ui/Button.hpp>
#include <ui/Text.hpp>
#include "../../../core/ServiceLocator.hpp"
#include "../../../core/commands/ui/ButtonClickedCommand.hpp"


namespace Engine::Systems {
    UiSystem::UiSystem() = default;

    UiSystem::~UiSystem() {
        for (const auto& cache_entry: m_font_textures | std::views::values) {
            m_render_controller->UnregisterTexture(cache_entry);
        }
    }

    void UiSystem::Initialize() {
        m_transform_cache = Cache()->GetTransformCache();
        m_ui_cache = Cache()->GetUiCache();
        m_text_controller = ServiceLocator()->GetService<Text::TextController>();
        m_render_controller = ServiceLocator()->GetService<Renderer::RenderController>();

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::UI::Button>(
                [this](const Ecs::EntityId entity, const Components::UI::Button& _) {
                    if (this->m_ui_cache == nullptr) {
                        throw std::runtime_error("UiSystem: Cache is null");
                    }
                    this->m_ui_cache->RegisterButtonEntity(entity);
                }
                );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::UI::Text>(
                [this](const Ecs::EntityId entity, const Components::UI::Text& _) {
                    if (this->m_ui_cache == nullptr) {
                        throw std::runtime_error("UiSystem: Cache is null");
                    }
                    this->m_ui_cache->RegisterTextEntity(entity);
                }
                );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::UI::Button>(
                [this](const Ecs::EntityId entity) {
                    this->m_ui_cache->DeregisterButtonEntity(entity);
                }
                );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::UI::Text>(
                [this](const Ecs::EntityId entity) {
                    this->m_ui_cache->DeregisterTextEntity(entity);
                }
                );
    }

    void UiSystem::Run(float delta_time) {
        HandleTextLabels();

        const Input::InputBuffer input = Input()->GetInput();
        if (!input.IsMapActive("UIInputMap")) {
            return;
        }
        HandleButtons(input);
    }

    bool UiSystem::IsMouseOverElement(const glm::vec2 mouse_pos, const Ecs::EntityId& rect_entity) const {
        const auto cached_values = m_transform_cache->GetRectTransformValue(rect_entity);
        if (mouse_pos.x > cached_values.global_position.x &&
            mouse_pos.y > cached_values.global_position.y &&
            mouse_pos.x < cached_values.global_position.x + cached_values.global_size.x &&
            mouse_pos.y < cached_values.global_position.y + cached_values.global_size.y) {
            return true;
        }
        return false;
    }

    void UiSystem::HandleButtons(const Input::InputBuffer& input) const {
        auto buttons_with_entities = EcsWorld()->GetComponentsOfType<Components::UI::Button>();
        for (auto [button, entity]: buttons_with_entities) {
            UI::UiCache::ButtonElement cached_button{};
            if (!button->enabled) {
                cached_button.color = button->disabled_color;
                m_ui_cache->SetButtonElementValue(entity, cached_button);
                continue;
            }

            cached_button.color = button->default_color;
            if (IsMouseOverElement(input.mouse_position, entity)) {
                cached_button.color = button->highlight_color;

                if (input.HasAction("UiButtonDown")) {
                    cached_button.color = button->click_color;
                } else if (input.HasAction("UiButtonUp")) {
                    const auto command = Core::Commands::UI::ButtonClickedCommand(button->button_id);
                    EcsWorld()->PushCommand(command);
                }
            }
            m_ui_cache->SetButtonElementValue(entity, cached_button);
        }
    }

    void UiSystem::HandleTextLabels() {
        auto text_labels = EcsWorld()->GetComponentsOfType<Components::UI::Text>();
        for (const auto [text, entity]: text_labels) {
            auto text_cache_value = m_ui_cache->GetTextElement(entity);

            if (text->GetFontVersion() != text_cache_value.last_font_version) {
                auto [font_handle, new_atlas_created] = m_text_controller->LoadFont(
                        text->GetFontName(),
                        text->GetFontSize()
                        );

                if (new_atlas_created && text_cache_value.texture_handle.has_value()) {
                    m_render_controller->UnregisterTexture(text_cache_value.texture_handle.value());
                }

                text_cache_value.font_handle = font_handle;
                text_cache_value.texture_handle = GetOrCreateTextureHandleFromFont(font_handle);
                text_cache_value.last_font_version = text->GetFontVersion();
                m_ui_cache->SetTextElementValue(entity, text_cache_value);
            }

            if (text->GetTextVersion() != text_cache_value.last_text_version) {
                if (text_cache_value.text_mesh.has_value()) {
                    m_render_controller->UnregisterMesh(text_cache_value.text_mesh.value());
                }
                if (!text_cache_value.font_handle.has_value()) {
                    continue;
                }
                auto text_mesh = m_text_controller->BuildTextMesh(text_cache_value.font_handle.value(),
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
                const auto mesh_handle = m_render_controller->RegisterMesh(text_mesh_asset);

                text_cache_value.text_mesh = mesh_handle;
                text_cache_value.last_text_version = text->GetTextVersion();
                m_ui_cache->SetTextElementValue(entity, text_cache_value);

                const auto rect_transform = EcsWorld()->GetComponent<Components::UI::RectTransform>(entity);
                rect_transform->SetSize(glm::vec2(text_mesh.dimensions_width, text_mesh.dimensions_height));
            }
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
