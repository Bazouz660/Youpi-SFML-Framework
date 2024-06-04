#pragma once

#include "../components/components.hpp"

#include "YPI/ecs.hpp"
#include "YPI/data.hpp"
#include "YPI/math.hpp"
#include "YPI/graphics.hpp"

#include "gridVal.h"

#include <future>

namespace sys
{

    class VerletSystem
    {
        public:

            VerletSystem()
            {
            }

            void update(entt::registry& registry, ypi::Grid<GridVal, GetBoxFunc, Equal, PairHash>& grid, float slowMotionFactor)
            {
                unsigned int subSteps = 8;
                float stepDt = (m_fixedTimeStep / subSteps);
                auto view = registry.view<comp::Verlet, comp::Transform>();

                for (unsigned int i(subSteps); i--;) {
                    // update the grid
                    view.each([&](auto entity, auto& verlet, auto& transform) {
                        grid.update({entity, &verlet});
                    });

                    view.each([&](auto entity, auto& verlet, auto& transform) {
                        applyGravity(entity, verlet);
                        updatePositions(entity, verlet, stepDt);
                        applyCircleConstraints(entity, verlet);
                        solveCollisions(entity, verlet, grid);
                        transform.setPosition(verlet.position);
                    });
                }
            }

        private:

            void updatePositions(const entt::entity& entity, comp::Verlet& verlet, float dt)
            {
                const float maxVelocity = 10.0f; // Adjust this value to change the maximum velocity

                ypi::Vector2f vel = (verlet.position - verlet.oldPosition);

                // Limit the velocity
                float velocityLength = ypi::math::length(vel);
                if (velocityLength > maxVelocity) {
                    vel = (vel / velocityLength) * maxVelocity;
                }

                verlet.oldPosition = verlet.position;
                verlet.position += vel + verlet.acceleration * dt * dt;

                verlet.acceleration = {0.0f, 0.0f};
            }

            void applyGravity(entt::entity& entity, comp::Verlet& verlet)
            {
                verlet.acceleration += {0.0f, m_gravity};
            }

            void applyCircleConstraints(entt::entity& entity, comp::Verlet& verlet)
            {
                const ypi::Vector2f center = {400.f, 300.f};
                const float radius = 300.0f;

                const ypi::Vector2f v = center - verlet.position;
                const float dist = ypi::math::length(v);
                if (dist > (radius - verlet.radius)) {
                    const ypi::Vector2f n = v / dist;
                    verlet.position = center - n * (radius - verlet.radius);
                }
            }

            void solveCollisions(const entt::entity& entity1, comp::Verlet& verlet1, ypi::Grid<GridVal, GetBoxFunc, Equal, PairHash>& grid)
            {
                    ypi::Rect<float> aabb1;
                    aabb1.left = verlet1.position.x - verlet1.radius;
                    aabb1.top = verlet1.position.y - verlet1.radius;
                    aabb1.width = verlet1.radius * 2;
                    aabb1.height = verlet1.radius * 2;

                    auto entities = grid.query(aabb1);
                    for (auto& [entity2, verlet2] : entities) {
                        if (entity1 == entity2)
                            continue;

                        // check if the two balls are near enough to collide on the x axis
                        float distX = abs(verlet1.position.x - verlet2->position.x);
                        float distY = abs(verlet1.position.y - verlet2->position.y);
                        if (distX > verlet1.radius + verlet2->radius || distY > verlet1.radius + verlet2->radius)
                            continue;

                        const ypi::Vector2f collisionAxis = verlet1.position - verlet2->position;
                        const float dist = ypi::math::length(collisionAxis);

                        if (dist < verlet1.radius + verlet2->radius) {
                            const ypi::Vector2f normal = collisionAxis / dist;
                            const float delta = (verlet1.radius + verlet2->radius - dist);

                            float totalMass = verlet1.mass + verlet2->mass;
                            verlet1.position += (verlet2->mass / totalMass) * delta * normal; // Adjust by mass here
                            verlet2->position -= (verlet1.mass / totalMass) * delta * normal; // Adjust by mass here
                        }
                    }
            }

            float m_accumulator = 0;
            float m_fixedTimeStep = 1.f / 60.f;
            float m_gravity = 1000.0f;
    };

}
