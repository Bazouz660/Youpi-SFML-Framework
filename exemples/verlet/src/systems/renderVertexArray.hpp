
#ifndef RENDER_VERTEX_ARRAY_SYSTEM_HPP_
#define RENDER_VERTEX_ARRAY_SYSTEM_HPP_

#include "../components/components.hpp"

#include "ypi/src/core/ecs/system/system.hpp"
#include "ypi/src/core/ecs/coordinator.hpp"
#include "ypi/src/core/window/Window.hpp"

namespace sys
{

    class RenderVertexArray : public exng::sys::System
    {
        public:
            RenderVertexArray(exng::Coordinator &coordinator) : System(coordinator) {}

            void render(exng::Window &window)
            {
                for (auto const &entity : mEntities) {
                    auto &drawable = mCoordinator.getComponent<comp::VertexArrayDrawable>(entity);
                    auto &transform = mCoordinator.getComponent<comp::Transform>(entity);

                    drawable.vertices.setPrimitiveType(drawable.primitiveType);

                    sf::RenderStates states;
                    states.transform.translate({transform.getPosition().x, transform.getPosition().y});
                    states.transform.rotate(transform.getRotation());
                    states.transform.scale(transform.getScale().x, transform.getScale().y);
                    states.transform.translate({-transform.getOrigin().x, -transform.getOrigin().y});

                    window.draw(drawable.vertices, states);
                }
            }
    };

}

#endif // !RENDER_VERTEX_ARRAY_SYSTEM_HPP_