//
// Created by Sebastian Borsch on 08.11.25.
//

#pragma once
#include <any>
#include <functional>

namespace Engine::SceneManagement {
    struct SceneArgs {
        std::any payload;
    };

    class IScene;
    using SceneFactory = std::function<std::unique_ptr<IScene>(const SceneArgs&)>;
}
