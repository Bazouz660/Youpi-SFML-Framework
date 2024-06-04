#include "Container.hpp"
#include "context.hpp"
#include "logger.hpp"

namespace ypi::gui
{
    void Container::addComponent(const std::string& key, const_sptr_ref<AComponent> component, int zIndex)
    {
        addChild(component);
        m_components[key] = std::make_pair(component, true);
        m_orderedComponents.push_back({component, true, zIndex});
        sortComponents();
    }

    void Container::removeComponent(const std::string& key)
    {
        if (m_components.find(key) == m_components.end()) {
            logger::error() << "Could not remove component with key " << key << ": it does not exist in the container.";
            return;
        }
        m_orderedComponents.erase(std::remove_if(m_orderedComponents.begin(), m_orderedComponents.end(), [this, key](const Component& c) {
            return c.component == m_components.at(key).first;
        }), m_orderedComponents.end());
        removeChild(m_components.at(key).first);
        m_components.erase(key);
        sortComponents();
    }

    void Container::sortComponents()
    {
        std::sort(m_orderedComponents.begin(), m_orderedComponents.end(), [](const Component& a, const Component& b) {
            int result = a.zIndex - b.zIndex;
            if (result == 0) // compare the addresses of the components if the z indices are the same to avoid z fighting
                return a.component.get() < b.component.get();
            return result < 0;
        });
    }

    void Container::render(RenderTarget& target )
    {
        for (auto& comp : m_orderedComponents) {
            if (comp.enabled && comp.component) {
                comp.component->render(target);
            }
        }
    }

    void Container::handleAnyEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        for (auto it = m_orderedComponents.rbegin(); it != m_orderedComponents.rend(); ++it) {
            auto& comp = *it;
            if (comp.enabled && comp.component) {
                comp.component->handleAnyEvents(event, mousePos);
            }
        }
    }

    void Container::handleHoverEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        for (auto it = m_orderedComponents.rbegin(); it != m_orderedComponents.rend(); ++it) {
            auto& comp = *it;
            if (comp.enabled && comp.component) {
                if (comp.component->getGlobalBounds().contains(mousePos)) {
                    comp.component->handleHoverEvents(event, mousePos);
                    return;
                }
            }
        }
    }

    bool Container::isComponentEnabled(const std::string& key) const
    {
        return m_components.at(key).second;
    }

    void Container::toggleComponent(const std::string& key, bool enabled)
    {
        m_components.at(key).second = enabled;
        m_orderedComponents.at(std::distance(m_orderedComponents.begin(), std::find_if(m_orderedComponents.begin(), m_orderedComponents.end(), [this, key](const Component& c) {
            return c.component == m_components.at(key).first;
        }))).enabled = enabled;
    }

    void Container::toggleComponent(const std::string& key)
    {
        toggleComponent(key, !isComponentEnabled(key));
    }

    void Container::clear()
    {
        m_components.clear();
        m_orderedComponents.clear();
    }

    FloatRect Container::getGlobalBounds() const
    {
        // combine the bounds of all the components
        std::unique_ptr<FloatRect> bounds = nullptr;
        for (auto& [key, pair] : m_components) {
            auto [component, enabled] = pair;
            if (enabled) {
                if (!bounds)
                    bounds = std::make_unique<FloatRect>(component->getGlobalBounds());
                else
                    *bounds = bounds->combine(component->getGlobalBounds());
            }
        }

        if (!bounds)
            return getShape().getGlobalBounds();

        return *bounds;
    }

    void Container::render(RenderTarget& target, const sf::View& view)
    {
        sf::View oldView = target.getView();
        target.setView(view);
        render(target);
        target.setView(oldView);
    }
}
