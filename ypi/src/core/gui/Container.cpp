#include "Container.hpp"

namespace exng::gui
{
    void Container::addComponent(std::shared_ptr<AComponent> component)
    {
        m_components.push_back(std::make_pair(component, true));
    }

    void Container::render(Window& window)
    {
        for (auto& [component, enabled] : m_components) {
            if (enabled)
                component->render(window);
        }
    }

    void Container::handleEvent(const sf::Event& event)
    {
        for (auto& [component, enabled] : m_components) {
            if (enabled)
                component->handleEvent(event);
        }
    }

    void Container::update()
    {
        for (auto& [component, enabled] : m_components) {
            if (enabled)
                component->update();
        }
    }

    void Container::setPosition(const sf::Vector2f& pos)
    {
        for (auto& [component, enabled] : m_components) {
            component->setPosition(pos);
        }
    }

    bool Container::isComponentEnabled(int index) const
    {
        return m_components.at(index).second;
    }

    void Container::toggleComponent(int index, bool enabled)
    {
        m_components.at(index).second = enabled;
    }
}
