#pragma once
#include <memory>

#include "EnvInput.hpp"
#include "IFileManager.hpp"
#include "Window.hpp"

namespace yarep::environment {
    struct EnvironmentBuilder {
        /**
         * Create a new window instance.
         * @return The new window instance as unique_ptr.
         */
        static std::unique_ptr<IWindow> CreateEngineWindow();

        static std::unique_ptr<IEnvInput> CreateInput(IWindow& window);

        static std::unique_ptr<files::IFileManager> CreateFileManager();
    };
}
