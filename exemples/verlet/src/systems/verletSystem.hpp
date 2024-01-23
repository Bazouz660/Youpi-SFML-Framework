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

            void update(float dt)
            {
                m_timer += dt;
                if (m_timer > float(1.f / 480.f)) {

                    const u_int32_t substeps = 6;
                    const float subDt = m_timer / float(substeps);

                    for (u_int32_t i(substeps); i--;) {
                        applyGravity();
                        applyCircleConstraints();
                        solveCollisions();
                        updatePositions(subDt);
                    }

                    m_timer = 0;
                }
            }

            void updatePositions(float dt)
            {
                for (auto& entity : mEntities) {
                    auto& verlet = mCoordinator.getComponent<comp::Verlet>(entity);
                    auto& transform = mCoordinator.getComponent<comp::Transform>(entity);

                    const exng::Vector2f vel = verlet.position - verlet.oldPosition;

                    verlet.oldPosition = verlet.position;
                    verlet.position += vel + verlet.acceleration * dt * dt;

                    verlet.acceleration = {0.0f, 0.0f};

                    transform.setPosition(verlet.position);
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
                const exng::Vector2f position = {400.f, 300.f};
                const float radius = 200.0f;

                for (auto& entity : mEntities) {
                    auto& verlet = mCoordinator.getComponent<comp::Verlet>(entity);
                    auto& transform = mCoordinator.getComponent<comp::Transform>(entity);

                    const exng::Vector2f delta = verlet.position - position;
                    const float deltaLength = exng::math::length(delta);

                    if (deltaLength > radius) {
                        const exng::Vector2f normal = delta / deltaLength;
                        verlet.position = position + normal * radius;
                    }

                    transform.setPosition(verlet.position);
                }
            }

            void solveCollisions()
            {
                for (auto& entity : mEntities) {
                    auto& verlet = mCoordinator.getComponent<comp::Verlet>(entity);
                    auto& transform = mCoordinator.getComponent<comp::Transform>(entity);

                    for (auto& otherEntity : mEntities) {
                        if (entity == otherEntity)
                            continue;

                        auto& otherVerlet = mCoordinator.getComponent<comp::Verlet>(otherEntity);
                        auto& otherTransform = mCoordinator.getComponent<comp::Transform>(otherEntity);

                        const exng::Vector2f collisionAxis = verlet.position - otherVerlet.position;
                        const float dist = exng::math::length(collisionAxis);

                        if (dist < verlet.radius + otherVerlet.radius) {

                            const exng::Vector2f normal = collisionAxis / dist;
                            const float delta = (verlet.radius + otherVerlet.radius - dist);

                            verlet.position += 0.5f * delta * normal;
                            otherVerlet.position -= 0.5f * delta * normal;
                        }

                        transform.setPosition(verlet.position);
                        otherTransform.setPosition(otherVerlet.position);
                    }
                }
            }

        private:
            float m_timer = 0;
    };

}
