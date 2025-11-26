//
// Created by Sebastian Borsch on 07.11.25.
//

#pragma once

namespace Engine {
    class IApplication {
    public:
        virtual ~IApplication() = default;

        virtual void Quit() = 0;
    };
}
