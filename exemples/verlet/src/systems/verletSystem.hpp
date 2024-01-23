#pragma once

#include "../components/components.hpp"

#include "ypi/src/core/ecs/system/system.hpp"
#include "ypi/src/core/ecs/coordinator.hpp"

namespace sys
{

    class VerletSystem : public exng::sys::System
    {
        public:
            VerletSystem(exng::Coordinator &coordinator) : System(coordinator) {}

            void update(float slowMotionFactor)
            {
                unsigned int subSteps = 8;
                float stepDt = (m_fixedTimeStep / subSteps);

                for (auto& entity : mEntities) {
                    auto& verlet = mCoordinator.getComponent<comp::Verlet>(entity);
                    m_entitiesPairs.push_back({entity, &verlet});
                }

                for (unsigned int i(subSteps); i--;) {
                    applyGravity();
                    updatePositions(stepDt);
                    applyCircleConstraints();
                    solveCollisions();
                }

                for (auto& [entity, verlet] : m_entitiesPairs) {
                    auto& transform = mCoordinator.getComponent<comp::Transform>(entity);

                    transform.setPosition(verlet->position);
                }

                m_entitiesPairs.clear();
            }

            void updatePositions(float dt)
            {
                const float maxVelocity = 10.0f; // Adjust this value to change the maximum velocity

                for (auto& entity : mEntities) {
                    auto& verlet = mCoordinator.getComponent<comp::Verlet>(entity);
                    auto& transform = mCoordinator.getComponent<comp::Transform>(entity);

                    exng::Vector2f vel = (verlet.position - verlet.oldPosition);

                    // Limit the velocity
                    float velocityLength = exng::math::length(vel);
                    if (velocityLength > maxVelocity) {
                        vel = (vel / velocityLength) * maxVelocity;
                    }

                    verlet.oldPosition = verlet.position;
                    verlet.position += vel + verlet.acceleration * dt * dt;

                    verlet.acceleration = {0.0f, 0.0f};
                }
            }

            void applyGravity()
            {
                for (auto& entity : mEntities) {
                    auto& verlet = mCoordinator.getComponent<comp::Verlet>(entity);

                    // apply gravity
                    verlet.acceleration += {0.0f, 1000};
                }
            }

            void applyCircleConstraints()
            {
                const exng::Vector2f center = {400.f, 300.f};
                const float radius = 300.0f;

                for (auto& entity : mEntities) {
                    auto& verlet = mCoordinator.getComponent<comp::Verlet>(entity);

                    const exng::Vector2f v = center - verlet.position;
                    const float dist = exng::math::length(v);
                    if (dist > (radius - verlet.radius)) {
                        const exng::Vector2f n = v / dist;
                        verlet.position = center - n * (radius - verlet.radius);
                    }
                }
            }

            void solveCollisions()
            {
                for (size_t i = 0; i < m_entitiesPairs.size(); ++i) {
                    for (size_t j = i + 1; j < m_entitiesPairs.size(); ++j) {
                        auto& verlet = *m_entitiesPairs[i].second;
                        auto& verlet2 = *m_entitiesPairs[j].second;

                        // check if the two balls are near enough to collide on the x axis
                        float distX = abs(verlet.position.x - verlet2.position.x);
                        float distY = abs(verlet.position.y - verlet2.position.y);
                        if (distX > verlet.radius + verlet2.radius || distY > verlet.radius + verlet2.radius)
                            continue;

                        const exng::Vector2f collisionAxis = verlet.position - verlet2.position;
                        const float dist = exng::math::length(collisionAxis);

                        if (dist < verlet.radius + verlet2.radius) {
                            const exng::Vector2f normal = collisionAxis / dist;
                            const float delta = (verlet.radius + verlet2.radius - dist);

                            float totalMass = verlet.mass + verlet2.mass;
                            verlet.position += (verlet2.mass / totalMass) * delta * normal; // Adjust by mass here
                            verlet2.position -= (verlet.mass / totalMass) * delta * normal; // Adjust by mass here
                        }
                    }
                }
            }

        private:
            float m_accumulator = 0;
            float m_fixedTimeStep = 1.f / 60.f;
            std::vector<std::pair<exng::Entity, comp::Verlet*>> m_entitiesPairs;
    };

}
