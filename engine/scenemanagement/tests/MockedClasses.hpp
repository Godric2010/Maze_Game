#pragma once
#include <IApplication.hpp>
#include <ISystemManager.hpp>
#include <Input/IInput.hpp>

#include "Assets/IAssetLibrary.hpp"

class FakeApplication : public Engine::IApplication
{
public:
    void Quit() override
    {
    }

    void RegisterScene(const std::string& name, Engine::SceneManagement::SceneFactory scene_factory) override
    {
    }

    void SetInitialScene(const std::string& name, const Engine::SceneManagement::SceneArgs& args) override
    {
    }
};

class FakeInput : public Engine::Input::IInput
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

    Engine::Input::InputBuffer GetInput() override
    {
        return {};
    }
};

class FakeSystemManager : public Engine::Ecs::ISystemManager
{
public:
    ~FakeSystemManager() override = default;

    void RegisterSystems(Engine::Ecs::World* world, Engine::Input::IInput* input) override
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

class FakeAssetLibrary : public Engine::Assets::IAssetLibrary
{
public:
    ~FakeAssetLibrary() override = default;

    Engine::Assets::MeshHandle LoadMesh(const std::string& name) override
    {
        return {};
    }

    Engine::Assets::MaterialHandle LoadMaterial(const std::string& name) override
    {
        return {};
    }

    Engine::Assets::TextureHandle LoadTexture(const std::string& name) override
    {
        return {};
    }

    std::optional<Engine::Assets::FontHandle> FindFont(const std::string& name) override
    {
        return {};
    }

    std::optional<Engine::Assets::MaterialHandle> FindMaterial(const std::string& name) override
    {
        return {};
    }

    std::optional<Engine::Assets::MeshHandle> FindMesh(const std::string& name) override
    {
        return {};
    }

    std::optional<Engine::Assets::TextureHandle> FindTexture(const std::string& name) override
    {
        return {};
    }

    std::optional<Engine::Assets::ShaderHandle> FindShader(const std::string& name) override
    {
        return {};
    }
};
