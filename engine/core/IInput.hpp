//
// Created by Sebastian Borsch on 04.11.25.
//

#pragma once
#include <string>

namespace Engine::Core {
    class IInput {
    public:
        virtual ~IInput() = default;

        virtual void EnableInputMap(const std::string &map_name) = 0;

        virtual void DisableInputMap(const std::string &map_name) = 0;

        virtual void SetMouseVisibility(bool visible) = 0;
    };
}
