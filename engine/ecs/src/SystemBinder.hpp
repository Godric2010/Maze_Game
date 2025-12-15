//
// Created by Sebastian Borsch on 15.12.25.
//

#pragma once
#include "EngineBindToken.hpp"
#include "Ecs/ISystem.hpp"

namespace Engine::Ecs {
    class SystemBinder {
    public:
        static void BindSystem(
                ISystem& system,
                Input::IInput& input,
                SystemWorld& game_world,
                const std::function<void(const std::any& command)>& func
                ) {
            constexpr EngineBindToken token;
            system.Bind(token, input, game_world, func);
        }
    };
}
