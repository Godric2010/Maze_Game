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

    void RegisterScene(const std::string& name, yarep::SceneManagement::SceneFactory scene_factory) override
    {
    }

    void SetInitialScene(const std::string& name, const yarep::SceneManagement::SceneArgs& args) override
    {
    }
};

class FakeInput : public yarep::Input::IInput
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

    yarep::Input::InputBuffer GetInput() override
    {
        return {};
    }
};

class FakeSystemManager : public yarep::Ecs::ISystemManager
{
public:
    ~FakeSystemManager() override = default;

    void RegisterSystems(yarep::Ecs::World* world, yarep::Input::IInput* input) override
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

class FakeAssetLibrary : public yarep::Assets::IAssetLibrary
{
public:
    ~FakeAssetLibrary() override = default;

    yarep::Assets::MeshHandle LoadMesh(const std::string& name) override
    {
        return {};
    }

    yarep::Assets::MaterialHandle LoadMaterial(const std::string& name) override
    {
        return {};
    }

    yarep::Assets::TextureHandle LoadTexture(const std::string& name) override
    {
        return {};
    }

    std::optional<yarep::Assets::FontHandle> FindFont(const std::string& name) override
    {
        return {};
    }

    std::optional<yarep::Assets::MaterialHandle> FindMaterial(const std::string& name) override
    {
        return {};
    }

    std::optional<yarep::Assets::MeshHandle> FindMesh(const std::string& name) override
    {
        return {};
    }

    std::optional<yarep::Assets::TextureHandle> FindTexture(const std::string& name) override
    {
        return {};
    }

    std::optional<yarep::Assets::ShaderHandle> FindShader(const std::string& name) override
    {
        return {};
    }
};
