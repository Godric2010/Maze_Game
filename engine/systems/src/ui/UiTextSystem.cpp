#include "UiTextSystem.hpp"
#include "ui/RectTransform.hpp"

using namespace Engine::Systems::UI;

namespace Engine::Systems
{
    UiTextSystem::UiTextSystem()
    {
    }

    UiTextSystem::~UiTextSystem()
    {
    }

    void UiTextSystem::Initialize()
    {
        m_transform_cache = Cache()->GetTransformCache();
        m_ui_cache = Cache()->GetUiCache();
        m_text_controller = ServiceLocator()->GetService<Text::TextController>();
        m_render_controller = ServiceLocator()->GetService<Renderer::IRenderController>();
        m_asset_handler = ServiceLocator()->GetService<AssetHandling::AssetHandler>();

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::UI::Text>(
             [this](const Ecs::EntityId entity, const Components::UI::Text& _)
             {
                 this->RegisterTextElement(entity);
             }

            );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::UI::Text>(
             [this](const Ecs::EntityId entity)
             {
                 this->m_ui_cache->DeregisterTextElement(entity);
             }
            );
    }

    void UiTextSystem::RegisterTextElement(Ecs::EntityId entity) const
    {
        if (this->m_ui_cache == nullptr)
        {
            throw std::runtime_error("UiSystem: Cache is null");
        }

        const auto text_mesh_asset = std::make_shared<AssetHandling::MeshAsset>();
        const auto mesh_handle = m_asset_handler->RegisterAsset(text_mesh_asset);
        auto asset = m_asset_handler->GetAsset<AssetHandling::MeshAsset>(mesh_handle);

        UiCache::TextElement text_element{};
        text_element.mesh_handle = mesh_handle;
        text_element.material_handle = RegisterNewUiMaterial();

        m_ui_cache->RegisterTextElement(entity, text_element);
    }

    Assets::MaterialHandle UiTextSystem::RegisterNewUiMaterial() const
    {
        const auto material_asset = std::make_shared<AssetHandling::MaterialAsset>();
        material_asset->name = std::string("UiTextMaterial");
        material_asset->render_state = AssetHandling::RenderState::UI;
        material_asset->render_queue_index = 0;
        material_asset->shader_handle = m_asset_handler->GetHandleFromName<AssetHandling::ShaderAsset>("ui");
        material_asset->albedo_texture = AssetHandling::MaterialTexture{};
        material_asset->base_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        const auto handle = m_asset_handler->RegisterAsset(material_asset);
        return handle;
    }

    void UiTextSystem::Run(float delta_time)
    {
        HandleTextLabels();
    }

    void UiTextSystem::HandleTextLabels()
    {
        auto text_labels = EcsWorld()->GetComponentsOfType<Components::UI::Text>();
        for (const auto [text, entity] : text_labels)
        {
            auto text_cache_value = m_ui_cache->GetTextElement(entity);

            if (text->GetFontVersion() != text_cache_value.last_font_version)
            {
                auto [font_handle, new_atlas_created] = m_text_controller->LoadFont(text->GetFontName(),
                         text->GetFontSize()
                    );

                bool font_texture_exists = m_font_textures.contains(font_handle);
                if (font_texture_exists)
                {
                    UpdateTextureFromFont(font_handle);
                }
                else
                {
                    RegisterTextureHandleFromFont(font_handle);
                }
                text_cache_value.font_handle = font_handle;
                text_cache_value.last_font_version = text->GetFontVersion();

                const auto material_asset = m_asset_handler->GetAsset<AssetHandling::MaterialAsset>(
                     text_cache_value.material_handle);
                auto font_texture_handle = m_font_textures[font_handle];
                if (material_asset->albedo_texture.texture != font_texture_handle)
                {
                    m_asset_handler->UpdateMaterial(text_cache_value.material_handle,
                                                    [font_texture_handle](AssetHandling::MaterialAsset& material)
                                                    {
                                                        material.albedo_texture.texture = font_texture_handle;
                                                        material.albedo_texture.uv_scale = glm::vec2(1, 1);
                                                        material.albedo_texture.tiling = glm::vec2(1, 1);
                                                    });
                }

                m_ui_cache->SetTextElementValue(entity, text_cache_value);
            }

            if (text->GetTextVersion() != text_cache_value.last_text_version)
            {
                UpdateTextMesh(entity, text_cache_value, text);
            }
        }
    }

    void UiTextSystem::UpdateTextMesh(const Ecs::EntityId entity, UiCache::TextElement text_element,
                                      const Components::UI::Text* text) const
    {
        if (!text_element.font_handle.has_value())
        {
            return;
        }
        auto text_mesh = m_text_controller->BuildTextMesh(text_element.font_handle.value(),
                                                          text->GetText(),
                                                          Text::TextAlignment::Left
                                                         );

        auto new_mesh_asset = AssetHandling::MeshAsset{};
        for (const auto& vertex : text_mesh.vertices)
        {
            AssetHandling::MeshVertexAsset mesh_vertex{
                .position = glm::vec3(vertex.x, vertex.y, 0),
                .uv = glm::vec2(vertex.u, vertex.v),
            };
            new_mesh_asset.vertices.emplace_back(mesh_vertex);
        }
        new_mesh_asset.indices = text_mesh.indices;
        m_asset_handler->UpdateMesh(text_element.mesh_handle, [new_mesh_asset](AssetHandling::MeshAsset& mesh_asset)
        {
            mesh_asset = new_mesh_asset;
        });

        text_element.last_text_version = text->GetTextVersion();
        m_ui_cache->SetTextElementValue(entity, text_element);

        const auto rect_transform = EcsWorld()->GetComponent<Components::UI::RectTransform>(entity);
        rect_transform->SetSize(glm::vec2(text_mesh.dimensions_width, text_mesh.dimensions_height));
    }

    void UiTextSystem::RegisterTextureHandleFromFont(Text::FontHandle font_handle)
    {
        const auto [width, height, pixels] = m_text_controller->GetTextureDescription(font_handle);
        auto texture_asset = std::make_shared<AssetHandling::TextureAsset>();
        texture_asset->width = width;
        texture_asset->height = height;
        texture_asset->format = AssetHandling::PixelFormat::R8;
        texture_asset->pixels = pixels;

        auto texture_handle = m_asset_handler->RegisterAsset(texture_asset);
        auto asset = m_asset_handler->GetAsset<AssetHandling::TextureAsset>(texture_handle);
        m_font_textures[font_handle] = texture_handle;
    }

    void UiTextSystem::UpdateTextureFromFont(Text::FontHandle font_handle)
    {
        auto texture_handle = m_font_textures[font_handle];
        const auto [width, height, pixels] = m_text_controller->GetTextureDescription(font_handle);
        auto new_texture_asset = AssetHandling::TextureAsset();
        new_texture_asset.width = width;
        new_texture_asset.height = height;
        new_texture_asset.format = AssetHandling::PixelFormat::R8;
        new_texture_asset.pixels = pixels;

        m_asset_handler->UpdateTexture(texture_handle, [new_texture_asset](AssetHandling::TextureAsset& texture_asset)
        {
            texture_asset = new_texture_asset;
        });
    }
} // namespace
