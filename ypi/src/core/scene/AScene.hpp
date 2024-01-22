#pragma once

#include <vector>
#include "core/ecs/entity/entity.hpp"
#include "core/ecs/coordinator.hpp"
#include "core/window/Window.hpp"

namespace exng
{

class SceneManager;

class AScene
{
    public:
        virtual void processEvents(sf::Event& event) = 0;
        virtual void update(float dt) = 0;
        virtual void render(Window& target) = 0;

        virtual void onEnter() = 0;
        virtual void onExit() = 0;

    protected:
        AScene(SceneManager& sceneManager)
            : m_sceneManager(sceneManager)
        {
            m_coordinator.init();
        }

        virtual ~AScene() = default;

        template<typename SystemType, typename... ComponentTypes>
        std::shared_ptr<SystemType> createSystem()
        {
            auto system = m_coordinator.registerSystem<SystemType>();

            Signature signature;
            (registerComponentType<ComponentTypes>(), ...);
            (signature.set(m_coordinator.getComponentType<ComponentTypes>()), ...);
            m_coordinator.setSystemSignature<SystemType>(signature);

            return system;
        }

        template<typename ComponentType>
        void registerComponentType()
        {
            if (!m_coordinator.isComponentRegistered<ComponentType>())
                m_coordinator.registerComponent<ComponentType>();
        }

        Coordinator m_coordinator;
        SceneManager& m_sceneManager;
};

}