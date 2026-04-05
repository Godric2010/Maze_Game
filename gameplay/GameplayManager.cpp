#include "GameplayManager.hpp"

#include "GameEndScene.hpp"
#include "GameScene.hpp"
#include "MainMenuScene.hpp"

namespace Gameplay
{
    GameplayManager::GameplayManager(Engine::IApplication& engine) : m_engine(engine)
    {
    }

    GameplayManager::~GameplayManager() = default;

    void GameplayManager::Initialize() const
    {
        m_engine.RegisterScene("MainMenu",
                               [](const Engine::SceneManagement::SceneArgs& args)
                               {
                                   return std::make_unique<MainMenuScene>();
                               }
        );

        m_engine.RegisterScene("Game",
                               [](const Engine::SceneManagement::SceneArgs& args)
                               {
                                   const auto game_scene_settings = std::any_cast<GameSceneSettings>(args.payload);
                                   return std::make_unique<GameScene>(game_scene_settings);
                               }
        );

        m_engine.RegisterScene("GameEnd",
                               [](const Engine::SceneManagement::SceneArgs& args)
                               {
                                   const auto game_end_data = std::any_cast<GameEndShowData>(args.payload);
                                   return std::make_unique<GameEndScene>(game_end_data);
                               }
        );

        m_engine.SetInitialScene("MainMenu", Engine::SceneManagement::SceneArgs{});
        // m_engine.SetInitialScene("Game", Engine::SceneManagement::SceneArgs{
        //                              GameSceneSettings{
        //                                  Difficulty::Developer
        //                              }
        //                          });
        // m_engine.SetInitialScene("GameEnd",
        //                          Engine::Input::SceneArgs{
        //                              GameEndShowData{m_mesh_handler.get(), 72.0f},
        //
        //                          }
        // );
    }


    void GameplayManager::Shutdown()
    {
    }
} // namespace
