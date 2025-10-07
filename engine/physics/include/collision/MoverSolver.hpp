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

            MoverResult out{position, false, std::numeric_limits<float>::infinity(), {}};

            for (int it = 0; it < input.max_iterations && length2(rest) > 1e-12f; ++it) {
                std::vector<Ecs::EntityId> candidates;
                query_service.QuerySphereSweep(position, rest, input.radius, candidates, nullptr);

                float best_time_of_impact = length(rest) + 1.0f;
                glm::vec3 best_normal(0);

                for (auto entity: candidates) {
                    const Math::AABB *box = query_service.GetAabb(entity);
                    if (!box) {
                        continue;
                    }
                    const auto hit = Sweep(Math::Sphere{position, input.radius}, rest, *box);
                    if (hit.hit && hit.time_of_impact < best_time_of_impact) {
                        best_time_of_impact = hit.time_of_impact;
                        best_normal = hit.normal;
                    }
                }
                if (best_time_of_impact <= length(rest)) {
                    glm::vec3 direction = normalize(rest);
                    position += direction * best_time_of_impact;
                    glm::vec3 remaining = rest - direction * best_time_of_impact;
                    rest = Math::Slide(remaining, best_normal);
                    out.collided = true;
                    out.first_time_of_impact = std::min(out.first_time_of_impact, best_time_of_impact);
                    out.last_normal = best_normal;
                } else {
                    position += rest;
                    rest = {};
                }
            }
            out.new_position = position;
            if (out.collided) {
                std::cout << "Collision detected!" << std::endl;
            }
            return out;
        }
    };
}
