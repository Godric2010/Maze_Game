//
// Created by Sebastian Borsch on 06.10.25.
//

#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include<glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "CollisionQueryService.hpp"
#include "math/Resolve.hpp"
#include "math/Sweep.hpp"

namespace Engine::Physics::Collision {
    struct MoverInput {
        glm::vec3 position;
        float radius;
        glm::vec3 delta;
        int max_iterations{3};
    };

    struct MoverResult {
        std::optional<Ecs::EntityId> hit_entity;
        glm::vec3 new_position;
        bool collided{};
        float first_time_of_impact{std::numeric_limits<float>::infinity()};
        glm::vec3 last_normal{};
    };

    class MoverSolver {
    public:
        static MoverResult Solve(const MoverInput &input, const ICollisionQueryService &query_service) {
            glm::vec3 position = input.position;
            glm::vec3 rest = input.delta;
            std::optional<Ecs::EntityId> hit_entity;

            MoverResult out{std::nullopt, position, false, std::numeric_limits<float>::infinity(), {}};

            for (int it = 0; it < input.max_iterations && length2(rest) > 1e-12f; ++it) {
                std::vector<Ecs::EntityId> candidates;
                query_service.QuerySphereSweep(position, rest, input.radius, candidates, nullptr);

                float best_time_of_impact = length(rest) + 1.0f;
                glm::vec3 best_normal(0);

                for (auto entity: candidates) {
                    Math::Sphere sphere(position, input.radius);

                    Math::CollisionHit hit;
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
                    glm::vec3 direction = normalize(rest);
                    position += direction * best_time_of_impact + best_normal * 1e-6f;
                    glm::vec3 remaining = rest - direction * best_time_of_impact;
                    rest = Math::Slide(remaining, best_normal);
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

        static inline bool TryGetCollisionHit(const Ecs::EntityId entity, const Math::Sphere sphere,
                                              const glm::vec3 &rest,
                                              const ICollisionQueryService &query_service,
                                              Math::CollisionHit &hit) {
            if (const auto *obb = query_service.GetObb(entity)) {
                hit = Sweep(sphere, rest, *obb);
                return true;
            }

            if (const auto *aabb = query_service.GetAabb(entity)) {
                hit = Sweep(sphere, rest, *aabb);
                return true;
            }

            return false;
        }
    };
}
