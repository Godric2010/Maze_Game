#include "UiImageSystem.hpp"

#include <TextController.hpp>
#include <ui/Button.hpp>
#include <ui/Text.hpp>
#include "ui/Image.hpp"


using namespace yarep::systems::ui;

namespace yarep::systems
{
    UiImageSystem::UiImageSystem() = default;

    UiImageSystem::~UiImageSystem()
    {
    }

    void UiImageSystem::Initialize()
    {
        m_transform_cache = Cache()->GetTransformCache();
        m_ui_cache = Cache()->GetUiCache();
        m_render_controller = ServiceLocator()->GetService<renderer::IRenderController>();
        m_asset_handler = ServiceLocator()->GetService<asset_handling::AssetHandler>();

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<components::ui::Image>(
             [this](const ecs::EntityId entity, const components::ui::Image& image)
             {
                 this->RegisterImageElement(entity, image.color);
             });


        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<components::ui::Image>(
             [this](const ecs::EntityId entity)
             {
                 this->m_ui_cache->DeregisterColorElement(entity);
             }
            );
    }

    void UiImageSystem::RegisterImageElement(const ecs::EntityId entity, const glm::vec4 color) const
    {
        if (this->m_ui_cache == nullptr)
        {
            throw std::runtime_error("UiSystem: Cache is null");
        }

        UiCache::ColorElement color_element{};
        color_element.color = color;
        color_element.mesh_handle = m_render_controller->GetUiMeshHandle();
        color_element.material_handle = RegisterNewUiMaterial();

        m_ui_cache->RegisterColorElement(entity, color_element);
    }

    assets::MaterialHandle UiImageSystem::RegisterNewUiMaterial() const
    {
        auto material_asset = asset_handling::MaterialAsset();
        material_asset.name = std::string("UiMaterial");
        material_asset.render_state = asset_handling::RenderState::UI;
        material_asset.render_queue_index = 0;
        material_asset.shader_handle = m_asset_handler->GetHandleFromName<asset_handling::ShaderAsset>("ui");
        material_asset.albedo_texture = asset_handling::MaterialTexture{};
        material_asset.base_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        const auto handle = m_asset_handler->RegisterAsset(material_asset);
        return handle;
    }

    void UiImageSystem::Run(float delta_time)
    {
        auto images_with_entities = EcsWorld()->GetComponentsOfType<components::ui::Image>();
        for (auto [image, entity] : images_with_entities)
        {
            UiCache::ColorElement color_element = m_ui_cache->GetColorElement(entity);
            color_element.color = image->color;
            m_ui_cache->SetColorElementValue(entity, color_element);
        }
    }
} // namespace
