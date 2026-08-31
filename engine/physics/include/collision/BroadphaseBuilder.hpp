//
// Created by sebastian on 07.02.26.
//

#pragma once
#include <memory>

#include "IBroadphase.hpp"


namespace yarep::physics::collision
{
    class BroadphaseBuilder
    {
    public:
        static std::unique_ptr<IBroadphase> BuildBroadphase(float cell_size);
    };
}
