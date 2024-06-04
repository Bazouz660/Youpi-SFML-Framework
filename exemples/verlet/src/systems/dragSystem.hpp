#pragma once

#include "YPI/ecs.hpp"
#include "YPI/data.hpp"
#include "YPI/math.hpp"
#include "YPI/graphics.hpp"

#include "../components/components.hpp"

namespace sys
{

    class DragSystem
    {
        public:
            DragSystem() = default;

            void update(entt::registry& registry, ypi::Grid<GridVal, GetBoxFunc, Equal, PairHash>& grid, ypi::RenderWindow &window)
            {
                ypi::Vector2f mousePosition = window.getMousePosition();

                auto entities = grid.query(mousePosition);

                bool found = false;
                for (auto const &[entity, verlet] : entities) {
                    auto& drawable = registry.get<comp::CircleDrawable>(entity);

                    bool isMouseOver = ypi::math::pointInCircle(mousePosition, verlet->position, verlet->radius);

                    if (isMouseOver) {
                        if (!mIsLastHovered) {
                            mLastHoveredEntity = entity;
                            mIsLastHovered = true;
                        } else {
                            if (mLastHoveredEntity != entity) {
                                auto& lastHoveredDrawable = registry.get<comp::CircleDrawable>(mLastHoveredEntity);
                                lastHoveredDrawable.shape.setOutlineThickness(0.0f);
                                mLastHoveredEntity = entity;
                            }
                        }
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

                if (mIsDragged && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    auto &verlet = registry.get<comp::Verlet>(mDraggedEntity);
                    float dist = ypi::math::dist(mousePosition, verlet.position);
                    if (dist > 1.0f) { // Avoid division by zero
                        ypi::Vector2f direction = (mousePosition - verlet.position);
                        ypi::Vector2f velocity = (verlet.position - verlet.oldPosition);
                        float dampingFactor = 0.9f; // Adjust this value to change the amount of damping
                        verlet.acceleration += direction * std::pow(dist, 2.f) * 10.f / dist - velocity * dampingFactor;
                    }
                } else {
                    mIsDragged = false;
                }
            }

        private:
            entt::entity mDraggedEntity;
            entt::entity mLastHoveredEntity;
            bool mIsDragged = false;
            bool mIsLastHovered = false;
    };

}