//
// Created by Sebastian Borsch on 11.01.26.
//

#include "EnvironmentBuilder.hpp"

#include <memory>

#include "IFileManager.hpp"
#include "SdlInput.hpp"
#include "SdlWindow.hpp"
#include "Window.hpp"
#include "Files/FileManager.hpp"

namespace yarep::environment {
    std::unique_ptr<IWindow> EnvironmentBuilder::CreateEngineWindow() {
        return std::make_unique<SdlWindow>();
    }

    std::unique_ptr<IEnvInput> EnvironmentBuilder::CreateInput(IWindow& window) {
        return std::make_unique<SdlInput>(reinterpret_cast<SdlWindow&>(window));
    }

    std::unique_ptr<files::IFileManager> EnvironmentBuilder::CreateFileManager() {
        return std::make_unique<files::FileManager>();
    }
}
