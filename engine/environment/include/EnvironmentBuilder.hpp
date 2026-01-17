#pragma once
#include <memory>

#include "EnvInput.hpp"
#include "IFileReader.hpp"
#include "Window.hpp"

namespace Engine::Environment {
    struct EnvironmentBuilder {
        /**
         * Create a new window instance.
         * @return The new window instance as unique_ptr.
         */
        static std::unique_ptr<IWindow> CreateEngineWindow();

        static std::unique_ptr<IEnvInput> CreateInput(IWindow& window);

        static std::unique_ptr<Files::IFileReader> CreateFileReader();
    };
}
