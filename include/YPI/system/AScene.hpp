#ifndef YPI_ASCENE_HPP
#define YPI_ASCENE_HPP

#include "../../lib_headers/common.hpp"
#include "../graphics/window/Window.hpp"

namespace ypi
{
    class SceneManager;

    class AScene
    {
        public:
            virtual ~AScene() = default;

            virtual void load() = 0;
            virtual void unload() = 0;
            virtual void update(float dt) = 0;
            virtual void handleEvent(const sf::Event& event, const Vector2f& mousePos) = 0;
            virtual void draw(ypi::RenderTexture& target ) = 0;

        protected:
            AScene(SceneManager& sceneManager)
                : m_sceneManager(sceneManager)
            {}
            SceneManager& m_sceneManager;
    };
}

#endif // !YPI_ASCENE_HPP