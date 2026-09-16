//
// Created by Sebastian Borsch on 06.10.25.
//

#pragma once
#include <optional>
#include <stdexcept>

#include "CollisionQueryService.hpp"
#include "Resolve.hpp"
#include "Sweep.hpp"

namespace yarep::physics::collision {
    struct MoverInput {
        math::Vec3 position;
        float radius{};
        math::Vec3 delta;
        int max_iterations{3};
    };

    struct MoverResult {
        std::optional<ecs::EntityId> hit_entity;
        math::Vec3 new_position;
        bool collided{};
        float first_time_of_impact{std::numeric_limits<float>::infinity()};
        math::Vec3 last_normal{};
    };

    class MoverSolver {
    public:
        static MoverResult Solve(const MoverInput& input, const ICollisionQueryService& query_service,
                                 const std::vector<ecs::EntityId>& candidates) {
            math::Vec3 position = input.position;
            math::Vec3 rest = input.delta;
            std::optional<ecs::EntityId> hit_entity;

            MoverResult out{std::nullopt, position, false, std::numeric_limits<float>::infinity(), {}};

            for (int it = 0; it < input.max_iterations && math::length_squared(rest) > 1e-12f; ++it) {
                float best_time_of_impact = length(rest) + 1.0f;
                math::Vec3 best_normal{};

                for (auto entity: candidates) {
                    geometry::Sphere sphere(position, input.radius);

                    CollisionHit hit;
                    if (!TryGetCollisionHit(entity, sphere, rest, query_service, hit)) {
                        throw std::runtime_error("No valid collider found");
                    }

                    if (hit.hit && hit.time_of_impact < best_time_of_impact) {
                        best_time_of_impact = hit.time_of_impact;
                        best_normal = hit.normal;
                        hit_entity = entity;
                    }
                }
                if (best_time_of_impact <= length(rest)) {
                    math::Vec3 direction = normalize(rest);
                    float skin = 0.001f;
                    position += direction * best_time_of_impact + best_normal * skin;
                    math::Vec3 remaining = rest - direction * best_time_of_impact;
                    rest = Slide(remaining, best_normal);
                    out.collided = true;
                    out.first_time_of_impact = std::min(out.first_time_of_impact, best_time_of_impact);
                    out.last_normal = best_normal;
                    out.hit_entity = hit_entity;
                } else {
                    position += rest;
                    rest = {};
                }
            }
            out.new_position = position;
            return out;
        }

        static inline bool TryGetCollisionHit(const ecs::EntityId entity, const geometry::Sphere sphere,
                                              const math::Vec3& rest,
                                              const ICollisionQueryService& query_service,
                                              CollisionHit& hit) {
            if (const auto* obb = query_service.GetObb(entity)) {
                hit = Sweep(sphere, rest, *obb);
                return true;
            }

            if (const auto* aabb = query_service.GetAabb(entity)) {
                hit = Sweep(sphere, rest, *aabb);
                return true;
            }

            return false;
        }
    };
}
