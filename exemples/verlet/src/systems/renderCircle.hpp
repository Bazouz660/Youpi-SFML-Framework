#pragma once

#include "../components/components.hpp"

#include "ypi/src/core/ecs/system/system.hpp"
#include "ypi/src/core/ecs/coordinator.hpp"
#include "ypi/src/core/window/Window.hpp"

namespace sys
{

    class RenderCircle : public exng::sys::System
    {
        public:
            RenderCircle(exng::Coordinator &coordinator) : System(coordinator) {}

            void render(exng::Window &window)
            {
                for (auto const &entity : mEntities) {
                    auto &drawable = mCoordinator.getComponent<comp::CircleDrawable>(entity);
                    auto &transform = mCoordinator.getComponent<comp::Transform>(entity);

                    drawable.shape.setPosition({transform.getPosition().x, transform.getPosition().y});
                    drawable.shape.setOrigin({transform.getOrigin().x, transform.getOrigin().y});
                    drawable.shape.setScale({transform.getScale().x, transform.getScale().y});
                    drawable.shape.setRotation(transform.getRotation());

                    window.draw(drawable.shape);
                }
            }
    };

}

