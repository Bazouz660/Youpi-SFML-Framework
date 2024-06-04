#include "ContainerGraph.hpp"

namespace ypi::gui
{
    void ContainerGraph::addNode(const_sptr_ref<Container> container, const_sptr_ref<Container> parent) {
        auto newNode = std::make_shared<Node>();
        newNode->parent = parent;
        if (parent) {
            auto it = m_nodesMap.find(parent);
            if (it != m_nodesMap.end()) {
                it->second->children.push_back(container);
            }
        }
        m_nodesMap[container] = newNode;
        if (!m_currentContainer) {
            m_currentContainer = container;
        }
    }

    void ContainerGraph::removeNode(const_sptr_ref<Container> container) {
        auto it = m_nodesMap.find(container);
        if (it != m_nodesMap.end()) {
            auto node = it->second;
            if (node->parent) {
                auto parentIt = m_nodesMap.find(node->parent);
                if (parentIt != m_nodesMap.end()) {
                    auto& siblings = parentIt->second->children;
                    siblings.erase(std::remove(siblings.begin(), siblings.end(), container), siblings.end());
                }
            }
            m_nodesMap.erase(it);
        }
    }

    void ContainerGraph::navigateToChild(int index) {
        if (m_currentContainer) {
            auto it = m_nodesMap.find(m_currentContainer);
            if (it != m_nodesMap.end() && index >= 0 && index < it->second->children.size()) {
                m_previousContainer = m_currentContainer;
                m_currentContainer = it->second->children[index];
            }
        }
    }

    void ContainerGraph::navigateToParent() {
        if (m_currentContainer) {
            auto it = m_nodesMap.find(m_currentContainer);
            if (it != m_nodesMap.end() && it->second->parent) {
                m_previousContainer = m_currentContainer;
                m_currentContainer = it->second->parent;
            }
        }
    }

    void ContainerGraph::handleAnyEvents(const sf::Event& event, const Vector2f& mousePos) {
        if (m_currentContainer) {
            m_currentContainer->handleHoverEvents(event, mousePos);
            m_currentContainer->handleAnyEvents(event, mousePos);
        }
    }

    void ContainerGraph::render(RenderTarget& target) {
        if (m_currentContainer) {
            m_currentContainer->render(target);
        }
    }

    FloatRect ContainerGraph::getGlobalBounds() const {
        if (m_currentContainer) {
            return m_currentContainer->getGlobalBounds();
        }
        return {};
    }

} // namespace ypi::gui