#include "UiButtonSystem.hpp"

#include "Commands/UI/ButtonClickedCommand.hpp"
#include "ui/Button.hpp"

using namespace Engine::Systems::UI;

namespace Engine::Systems
{
    UiButtonSystem::UiButtonSystem()
    {
    }

    UiButtonSystem::~UiButtonSystem()
    {
    }

    void UiButtonSystem::Initialize()
    {
        m_transform_cache = Cache()->GetTransformCache();
        m_ui_cache = Cache()->GetUiCache();
        m_render_controller = ServiceLocator()->GetService<Renderer::IRenderController>();
        m_asset_handler = ServiceLocator()->GetService<AssetHandling::AssetHandler>();

        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentAddEvent<Components::UI::Button>(
            [this](const Ecs::EntityId entity, const Components::UI::Button& button)
            {
                this->RegisterButtonElement(entity, button.default_color);
            }
        );
        EcsWorld()->GetComponentEventBus()->SubscribeOnComponentRemoveEvent<Components::UI::Button>(
            [this](const Ecs::EntityId entity)
            {
                this->m_ui_cache->DeregisterColorElement(entity);
            }
        );
    }

    void UiButtonSystem::Run(float delta_time)
    {
        const Input::InputBuffer input = Input()->GetInput();
        if (!input.IsMapActive("UIInputMap"))
        {
            return;
        }
        HandleButtons(input);
    }

    void UiButtonSystem::RegisterButtonElement(const Ecs::EntityId entity, const glm::vec4 color) const
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

    Assets::MaterialHandle UiButtonSystem::RegisterNewUiMaterial() const
    {
        const auto material_asset = std::make_shared<AssetHandling::MaterialAsset>();
        material_asset->name = std::string("UiMaterial");
        material_asset->render_state = AssetHandling::RenderState::UI;
        material_asset->render_queue_index = 0;
        material_asset->shader_handle = m_asset_handler->GetHandleFromName<AssetHandling::ShaderAsset>("ui");
        material_asset->albedo_texture = AssetHandling::MaterialTexture{};
        material_asset->base_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        const auto handle = m_asset_handler->RegisterAsset(material_asset);
        return handle;
    }

    bool UiButtonSystem::IsMouseOverElement(glm::vec2 mouse_pos, const Ecs::EntityId& rect_entity) const
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

    void UiButtonSystem::HandleButtons(const Input::InputBuffer& input) const
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
} // namespace
