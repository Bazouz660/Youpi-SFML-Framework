#pragma once

#include "ypi/src/core/ecs/system/system.hpp"
#include "ypi/src/core/ecs/coordinator.hpp"
#include "ypi/src/core/window/Window.hpp"
#include "ypi/src/helper/math/math.hpp"

#include "../components/components.hpp"

namespace sys
{

    class DragSystem : public exng::sys::System
    {
        public:
            DragSystem(exng::Coordinator &coordinator) : System(coordinator) {}

            void update(exng::Window &window)
            {
                exng::Vector2f mousePosition = window.getMousePosition();

                for (auto const &entity : mEntities) {
                    auto &verlet = mCoordinator.getComponent<comp::Verlet>(entity);
                    auto &drawable = mCoordinator.getComponent<comp::CircleDrawable>(entity);

                    bool isMouseOver = exng::math::pointInCircle(mousePosition, verlet.position, verlet.radius);

                    if (isMouseOver) {
                        drawable.shape.setOutlineColor(sf::Color::Red);
                        drawable.shape.setOutlineThickness(1.0f);
                    } else {
                        drawable.shape.setOutlineThickness(0.0f);
                    }

                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        if (!mIsDragged && isMouseOver) {
                            mDraggedEntity = entity;
                            mIsDragged = true;
                        }
                    } else {
                        mIsDragged = false;
                    }
                }

                if (mIsDragged) {
                    auto &verlet = mCoordinator.getComponent<comp::Verlet>(mDraggedEntity);
                    float dist = exng::math::dist(mousePosition, verlet.position);
                    if (dist > 1.0f) { // Avoid division by zero
                        exng::Vector2f direction = (mousePosition - verlet.position);
                        exng::Vector2f velocity = (verlet.position - verlet.oldPosition);
                        float dampingFactor = 0.9f; // Adjust this value to change the amount of damping
                        verlet.acceleration += direction * std::pow(dist, 2.f) * 10.f / dist - velocity * dampingFactor;
                    }
                }
            }

        private:
            exng::Entity mDraggedEntity;
            bool mIsDragged = false;
    };

}