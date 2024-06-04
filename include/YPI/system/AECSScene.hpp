#ifndef YPI_AECSSCENE_HPP
#define YPI_AECSSCENE_HPP

#include "AScene.hpp"
#include "../../lib_headers/entt.hpp"

namespace ypi
{
    class AECSScene : public AScene
    {
        public:
            virtual ~AECSScene() = default;

            virtual void load() override = 0;
            virtual void unload() override = 0;
            virtual void update(float dt) override = 0;
            virtual void handleEvent(const sf::Event& event, const Vector2f& mousePos) override = 0;
            virtual void draw(ypi::RenderTexture& target ) override = 0;

            entt::registry& getRegistry() { return m_registry; }

        protected:
            AECSScene(SceneManager& sceneManager)
                : AScene(sceneManager)
            {}
            entt::registry m_registry;
    };
}

#endif // !YPI_AECSSCENE_HPP
