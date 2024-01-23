
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
                    auto &verlet = mCoordinator.getComponent<comp::Verlet>(entity);

                    drawable.vertices.setPrimitiveType(drawable.primitiveType);

                    sf::RenderStates states;
                    states.transform.translate({verlet.position.x, verlet.position.y});

                    window.draw(drawable.vertices, states);
                }
            }
    };

}

#endif // !RENDER_VERTEX_ARRAY_SYSTEM_HPP_