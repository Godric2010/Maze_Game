//
// Created by Sebastian Borsch on 11.01.26.
//

#include "EnvironmentBuilder.hpp"

#include <memory>

#include "IFileReader.hpp"
#include "SDLInput.hpp"
#include "SDLWindow.hpp"
#include "Window.hpp"
#include "Files/FileReader.hpp"

namespace Engine::Environment {
    std::unique_ptr<IWindow> EnvironmentBuilder::CreateEngineWindow() {
        return std::make_unique<SDLWindow>();
    }

    std::unique_ptr<IEnvInput> EnvironmentBuilder::CreateInput(IWindow& window) {
        return std::make_unique<SDLInput>(reinterpret_cast<SDLWindow&>(window));
    }

    std::unique_ptr<Files::IFileReader> EnvironmentBuilder::CreateFileReader() {
        return std::make_unique<Files::FileReader>();
    }
}
