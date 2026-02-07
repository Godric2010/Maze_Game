#include "collision/BroadphaseBuilder.hpp"

#include "SpatialHashBroadphase.hpp"

namespace Engine::Physics::Collision
{
    std::unique_ptr<IBroadphase> BroadphaseBuilder::BuildBroadphase(float cell_size)
    {
        return std::make_unique<SpatialHashBroadphase>(cell_size);
    }
} // namespace
