//
// Created by sebastian on 07.02.26.
//

#pragma once
#include <memory>

#include "IBroadphase.hpp"


namespace yarep::Physics::Collision
{
    class BroadphaseBuilder
    {
    public:
        static std::unique_ptr<IBroadphase> BuildBroadphase(float cell_size);
    };
}
