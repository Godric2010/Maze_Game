//
// Created by Sebastian Borsch on 11.01.26.
//

#include "EnvironmentBuilder.hpp"

#include <memory>

#include "IFileManager.hpp"
#include "SDLInput.hpp"
#include "SDLWindow.hpp"
#include "Window.hpp"
#include "Files/FileManager.hpp"

namespace Engine::Environment {
    std::unique_ptr<IWindow> EnvironmentBuilder::CreateEngineWindow() {
        return std::make_unique<SDLWindow>();
    }

    std::unique_ptr<IEnvInput> EnvironmentBuilder::CreateInput(IWindow& window) {
        return std::make_unique<SDLInput>(reinterpret_cast<SDLWindow&>(window));
    }

    std::unique_ptr<Files::IFileManager> EnvironmentBuilder::CreateFileManager() {
        return std::make_unique<Files::FileManager>();
    }
}
