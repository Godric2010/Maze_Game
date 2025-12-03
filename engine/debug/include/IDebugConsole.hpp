//
// Created by Sebastian Borsch on 03.12.25.
//

#pragma once
#include <string>

namespace Engine::Debug {
    class IDebugConsole {
    public:
        virtual ~IDebugConsole() = default;

        virtual void PushValue(const std::string& label, size_t value) = 0;

        virtual void PushToFrame() = 0;
    };
}
