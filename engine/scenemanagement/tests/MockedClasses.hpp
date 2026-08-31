#pragma once
#include <IApplication.hpp>
#include <ISystemManager.hpp>
#include <Input/IInput.hpp>

#include "Assets/IAssetLibrary.hpp"

class FakeApplication : public yarep::IApplication
{
public:
    void Quit() override
    {
    }

    void RegisterScene(const std::string& name, yarep::scene_management::SceneFactory scene_factory) override
    {
    }

    void SetInitialScene(const std::string& name, const yarep::scene_management::SceneArgs& args) override
    {
    }
};

class FakeInput : public yarep::input::IInput
{
public:
    void EnableInputMap(const std::string& map_name) override
    {
    }

    void DisableInputMap(const std::string& map_name) override
    {
    }

    void SetMouseVisibility(bool visible) override
    {
    }

    yarep::input::InputBuffer GetInput() override
    {
        return {};
    }
};

class FakeSystemManager : public yarep::ecs::ISystemManager
{
public:
    ~FakeSystemManager() override = default;

    void RegisterSystems(yarep::ecs::World* world, yarep::input::IInput* input) override
    {
    }

    void PreFixed(float delta_time) override
    {
    }

    void FixedUpdateSystems(float fixed_dt) override
    {
    }

    void UpdateSystems(float delta_time) override
    {
    }

    void RegisterForSystemCommands(std::string subscriber_name,
                                   std::function<void(std::vector<std::any>)> command_callback) override
    {
    }

    void DeregisterForSystemCommands(const std::string& subscriber_name) override
    {
    }
};

class FakeAssetLibrary : public yarep::assets::IAssetLibrary
{
public:
    ~FakeAssetLibrary() override = default;

    yarep::assets::MeshHandle LoadMesh(const std::string& name) override
    {
        return {};
    }

    yarep::assets::MaterialHandle LoadMaterial(const std::string& name) override
    {
        return {};
    }

    yarep::assets::TextureHandle LoadTexture(const std::string& name) override
    {
        return {};
    }

    std::optional<yarep::assets::FontHandle> FindFont(const std::string& name) override
    {
        return {};
    }

    std::optional<yarep::assets::MaterialHandle> FindMaterial(const std::string& name) override
    {
        return {};
    }

    std::optional<yarep::assets::MeshHandle> FindMesh(const std::string& name) override
    {
        return {};
    }

    std::optional<yarep::assets::TextureHandle> FindTexture(const std::string& name) override
    {
        return {};
    }

    std::optional<yarep::assets::ShaderHandle> FindShader(const std::string& name) override
    {
        return {};
    }
};
