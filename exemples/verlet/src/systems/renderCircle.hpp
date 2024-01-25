#pragma once

#include "../components/components.hpp"

#include "ypi/src/core/ecs/system/system.hpp"
#include "ypi/src/core/ecs/coordinator.hpp"
#include "ypi/src/core/window/Window.hpp"

#include "ypi/lib_headers/entt.hpp"

namespace sys
{

    class RenderCircle
    {
        public:
            RenderCircle() = default;

            void render(entt::registry& registry, exng::Window &window)
            {
                auto view = registry.view<comp::CircleDrawable, comp::Transform>();

                view.each([&](auto entity, auto& drawable, auto& transform) {
                    drawable.shape.setPosition({transform.getPosition().x, transform.getPosition().y});
                    drawable.shape.setOrigin({transform.getOrigin().x, transform.getOrigin().y});
                    drawable.shape.setScale({transform.getScale().x, transform.getScale().y});
                    drawable.shape.setRotation(transform.getRotation());

                    window.draw(drawable.shape);
                });
            }
    };

}

