#include "UiSystem.hpp"

#include <TextController.hpp>
#include <ui/Button.hpp>
#include <ui/Text.hpp>
#include "Commands/UI/ButtonClickedCommand.hpp"
#include "ui/Image.hpp"


using namespace Engine::Systems::UI;

namespace Engine::Systems
{
    UiSystem::UiSystem() = default;

    UiSystem::~UiSystem()
    {
        for (const auto& cache_entry : m_font_textures | std::views::values)
        {
            m_render_controller->UnregisterTexture(cache_entry);
        }
    }

    void UiSystem::Initialize()
    {
        m_transform_cache = Cache()->GetTransformCache();
        m_ui_cache = Cache()->GetUiCache();
        m_text_controller = ServiceLocator()->GetService<Text::TextController>();
        m_render_controller = ServiceLocator()->GetService<Renderer::IRenderController>();
        m_asset_handler = ServiceLocator()->GetService<AssetHandling::AssetHandler>();

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::UI::Button>(
            [this](const Ecs::EntityId entity, const Components::UI::Button& button)
            {
                this->RegisterColorElement(entity, button.default_color);
            }
        );

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::UI::Image>(
            [this](const Ecs::EntityId entity, const Components::UI::Image& image)
            {
                this->RegisterColorElement(entity, image.color);
            });

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::UI::Text>(
            [this](const Ecs::EntityId entity, const Components::UI::Text& _)
            {
                this->RegisterTextElement(entity);
            }
        );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::UI::Button>(
            [this](const Ecs::EntityId entity)
            {
                this->m_ui_cache->DeregisterColorElement(entity);
            }
        );

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::UI::Image>(
            [this](const Ecs::EntityId entity)
            {
                this->m_ui_cache->DeregisterColorElement(entity);
            }
        );

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::UI::Text>(
            [this](const Ecs::EntityId entity)
            {
                this->m_ui_cache->DeregisterTextElement(entity);
            }
        );
    }

    void UiSystem::RegisterColorElement(Ecs::EntityId entity, glm::vec4 color)
    {
        if (this->m_ui_cache == nullptr)
        {
            throw std::runtime_error("UiSystem: Cache is null");
        }

        UiCache::ColorElement color_element{};
        color_element.color = color;
        color_element.mesh_handle = m_render_controller->GetUIMeshHandle();
        color_element.material_handle = RegisterNewUiMaterial();

        m_ui_cache->RegisterColorElement(entity, color_element);
    }

    void UiSystem::RegisterTextElement(const Ecs::EntityId entity) const
    {
        if (this->m_ui_cache == nullptr)
        {
            throw std::runtime_error("UiSystem: Cache is null");
        }

        const auto text_mesh_asset = std::make_shared<AssetHandling::MeshAsset>();
        const auto mesh_handle = m_asset_handler->RegisterAsset(text_mesh_asset);
        auto asset = m_asset_handler->GetAsset<AssetHandling::MeshAsset>(mesh_handle);
        m_render_controller->RegisterMesh(*asset, mesh_handle);

        UiCache::TextElement text_element{};
        text_element.mesh_handle = mesh_handle;
        text_element.material_handle = RegisterNewUiMaterial();

        m_ui_cache->RegisterTextElement(entity, text_element);
    }

    Assets::MaterialHandle UiSystem::RegisterNewUiMaterial() const
    {
        const auto material_asset = std::make_shared<AssetHandling::MaterialAsset>();
        material_asset->name = std::string("UiMaterial");
        material_asset->render_state = AssetHandling::RenderState::UI;
        material_asset->render_queue_index = 0;
        material_asset->shader_handle = m_asset_handler->GetHandleFromName<AssetHandling::ShaderAsset>("ui");
        material_asset->albedo_texture = AssetHandling::MaterialTexture{};
        material_asset->base_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        const auto handle = m_asset_handler->RegisterAsset(material_asset);
        m_render_controller->RegisterMaterial(handle);
        return handle;
    }

    void UiSystem::Run(float delta_time)
    {
        HandleTextLabels();

        const Input::InputBuffer input = Input()->GetInput();
        if (!input.IsMapActive("UIInputMap"))
        {
            return;
        }
        HandleButtons(input);
    }


    bool UiSystem::IsMouseOverElement(const glm::vec2 mouse_pos, const Ecs::EntityId& rect_entity) const
    {
        const auto cached_values = m_transform_cache->GetRectTransformValue(rect_entity);
        if (mouse_pos.x > cached_values.global_position.x &&
            mouse_pos.y > cached_values.global_position.y &&
            mouse_pos.x < cached_values.global_position.x + cached_values.global_size.x &&
            mouse_pos.y < cached_values.global_position.y + cached_values.global_size.y)
        {
            return true;
        }
        return false;
    }

    void UiSystem::HandleButtons(const Input::InputBuffer& input) const
    {
        auto buttons_with_entities = EcsWorld()->GetComponentsOfType<Components::UI::Button>();
        for (auto [button, entity] : buttons_with_entities)
        {
            UiCache::ColorElement cached_button = m_ui_cache->GetColorElement(entity);
            if (!button->enabled)
            {
                cached_button.color = button->disabled_color;
                m_ui_cache->SetColorElementValue(entity, cached_button);
                continue;
            }

            cached_button.color = button->default_color;
            if (IsMouseOverElement(input.mouse_position, entity))
            {
                cached_button.color = button->highlight_color;

                if (input.HasAction("UiButtonDown"))
                {
                    cached_button.color = button->click_color;
                }
                else if (input.HasAction("UiButtonUp"))
                {
                    const auto command = Commands::UI::ButtonClickedCommand(button->button_id);
                    EcsWorld()->PushCommand(command);
                }
            }
            m_ui_cache->SetColorElementValue(entity, cached_button);
        }
    }

    void UiSystem::HandleTextLabels()
    {
        auto text_labels = EcsWorld()->GetComponentsOfType<Components::UI::Text>();
        for (const auto [text, entity] : text_labels)
        {
            auto text_cache_value = m_ui_cache->GetTextElement(entity);

            if (text->GetFontVersion() != text_cache_value.last_font_version)
            {
                auto [font_handle, new_atlas_created] = m_text_controller->LoadFont(
                    text->GetFontName(),
                    text->GetFontSize()
                );
                const auto material_asset = m_asset_handler->GetAsset<AssetHandling::MaterialAsset>(
                    text_cache_value.material_handle);
                if (new_atlas_created && material_asset->albedo_texture.texture)
                {
                    m_render_controller->UnregisterTexture(material_asset->albedo_texture.texture);
                }

                text_cache_value.font_handle = font_handle;
                text_cache_value.last_font_version = text->GetFontVersion();


                const auto font_texture_handle = GetOrCreateTextureHandleFromFont(font_handle);
                material_asset->albedo_texture.texture = font_texture_handle;
                m_render_controller->UnregisterMaterial(text_cache_value.material_handle);
                m_render_controller->RegisterMaterial(text_cache_value.material_handle);


                m_ui_cache->SetTextElementValue(entity, text_cache_value);
            }

            if (text->GetTextVersion() != text_cache_value.last_text_version)
            {
                BuildNewTextMesh(entity, text_cache_value, text);
            }
        }
    }

    void UiSystem::BuildNewTextMesh(const Ecs::EntityId entity, UiCache::TextElement text_element,
                                    const Components::UI::Text* text) const
    {
        if (text_element.mesh_handle != Assets::MeshHandle{})
        {
            m_render_controller->UnregisterMesh(text_element.mesh_handle);
        }
        if (!text_element.font_handle.has_value())
        {
            return;
        }
        auto text_mesh = m_text_controller->BuildTextMesh(text_element.font_handle.value(),
                                                          text->GetText(),
                                                          Text::TextAlignment::Left
        );

        const auto text_mesh_asset = m_asset_handler->GetAsset<AssetHandling::MeshAsset>(text_element.mesh_handle);
        for (const auto& vertex : text_mesh.vertices)
        {
            AssetHandling::MeshVertexAsset mesh_vertex{
                .position = glm::vec3(vertex.x, vertex.y, 0),
                .uv = glm::vec2(vertex.u, vertex.v),
            };
            text_mesh_asset->vertices.emplace_back(mesh_vertex);
        }
        text_mesh_asset->indices = text_mesh.indices;
        m_render_controller->RegisterMesh(*text_mesh_asset, text_element.mesh_handle);

        text_element.last_text_version = text->GetTextVersion();
        m_ui_cache->SetTextElementValue(entity, text_element);

        const auto rect_transform = EcsWorld()->GetComponent<Components::UI::RectTransform>(entity);
        rect_transform->SetSize(glm::vec2(text_mesh.dimensions_width, text_mesh.dimensions_height));
    }

    Assets::TextureHandle UiSystem::GetOrCreateTextureHandleFromFont(const Text::FontHandle font_handle)
    {
        if (m_font_textures.contains(font_handle))
        {
            return m_font_textures.at(font_handle);
        }
        const auto [width, height, pixels] = m_text_controller->GetTextureDescription(font_handle);
        auto texture_asset = std::make_shared<AssetHandling::TextureAsset>();
        texture_asset->width = width;
        texture_asset->height = height;
        texture_asset->format = AssetHandling::PixelFormat::R8;
        texture_asset->pixels = pixels;

        auto texture_handle = m_asset_handler->RegisterAsset(texture_asset);
        auto asset = m_asset_handler->GetAsset<AssetHandling::TextureAsset>(texture_handle);
        m_font_textures[font_handle] = texture_handle;
        m_render_controller->RegisterTexture(*asset, texture_handle);
        return texture_handle;
    }
} // namespace
