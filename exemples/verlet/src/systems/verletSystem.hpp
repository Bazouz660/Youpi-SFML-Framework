#pragma once

#include "../components/components.hpp"

#include "ypi/src/core/ecs/system/system.hpp"
#include "ypi/src/core/ecs/coordinator.hpp"
#include "ypi/src/core/space_partitionning/Grid.hpp"
#include "ypi/src/core/rect/Rect.hpp"

namespace sys
{

    using GridVal = std::pair<exng::Entity, comp::Verlet*>;
    using GetBoxFunc = std::function<exng::Rect<float>(const GridVal&)>;

    class VerletSystem : public exng::sys::System
    {
        public:
            VerletSystem(exng::Coordinator &coordinator)
                : System(coordinator), m_grid(800, 600, 20, [&](const GridVal& entity) {
                    exng::Rect<float> aabb;
                    auto verlet = entity.second;
                    aabb.left = verlet->position.x - verlet->radius;
                    aabb.top = verlet->position.y - verlet->radius;
                    aabb.width = verlet->radius * 2;
                    aabb.height = verlet->radius * 2;
                    return aabb;
                })
            {
            }

            void update(float slowMotionFactor)
            {
                unsigned int subSteps = 8;
                float stepDt = (m_fixedTimeStep / subSteps);

                for (auto& entity : mEntities) {
                    auto& verlet = mCoordinator.getComponent<comp::Verlet>(entity);
                    m_entitiesPairs.push_back({entity, &verlet});
                }

                // update the grid
                m_grid.clear();
                for (size_t i = 0; i < m_entitiesPairs.size(); ++i) {
                    m_grid.insert(m_entitiesPairs[i]);
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
                    auto& [entity1, verlet1] = m_entitiesPairs[i];

                    exng::Rect<float> aabb1;
                    aabb1.left = verlet1->position.x - verlet1->radius;
                    aabb1.top = verlet1->position.y - verlet1->radius;
                    aabb1.width = verlet1->radius * 2;
                    aabb1.height = verlet1->radius * 2;

                    auto entities = m_grid.query(aabb1);
                    //exng::logger::debug() << "entities.size() = " << entities.size();
                    for (auto& [entity2, verlet2] : entities) {
                        if (entity1 == entity2)
                            continue;

                        // check if the two balls are near enough to collide on the x axis
                        float distX = abs(verlet1->position.x - verlet2->position.x);
                        float distY = abs(verlet1->position.y - verlet2->position.y);
                        if (distX > verlet1->radius + verlet2->radius || distY > verlet1->radius + verlet2->radius)
                            continue;

                        const exng::Vector2f collisionAxis = verlet1->position - verlet2->position;
                        const float dist = exng::math::length(collisionAxis);

                        if (dist < verlet1->radius + verlet2->radius) {
                            const exng::Vector2f normal = collisionAxis / dist;
                            const float delta = (verlet1->radius + verlet2->radius - dist);

                            float totalMass = verlet1->mass + verlet2->mass;
                            verlet1->position += (verlet2->mass / totalMass) * delta * normal; // Adjust by mass here
                            verlet2->position -= (verlet1->mass / totalMass) * delta * normal; // Adjust by mass here
                        }
                    }
                }
            }

        private:
            float m_accumulator = 0;
            float m_fixedTimeStep = 1.f / 60.f;
            std::vector<GridVal> m_entitiesPairs;

            exng::Grid<GridVal, GetBoxFunc> m_grid;
    };

}
