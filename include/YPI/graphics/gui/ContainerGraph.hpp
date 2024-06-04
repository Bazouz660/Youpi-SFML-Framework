#ifndef YPI_GUI_CONTAINER_GRAPH_HPP
#define YPI_GUI_CONTAINER_GRAPH_HPP

#include "Container.hpp"

namespace ypi::gui
{
    class ContainerGraph : virtual public AComponent
    {
        public:
            struct Node
            {
                std::shared_ptr<Container> parent = nullptr;
                std::vector<std::shared_ptr<Container>> children;
            };

            void addNode(const_sptr_ref<Container> container, const_sptr_ref<Container> parent = nullptr);
            void removeNode(const_sptr_ref<Container> node);
            void navigateToChild(int index);
            void navigateToParent();

            virtual void handleAnyEvents(const sf::Event& event, const Vector2f& mousePos) override;
            virtual void render(RenderTarget& target) override;

            virtual FloatRect getGlobalBounds() const override;

        protected:
            std::shared_ptr<Container> m_currentContainer = nullptr;
            std::shared_ptr<Container> m_previousContainer = nullptr;
            std::unordered_map<std::shared_ptr<Container>, std::shared_ptr<Node>> m_nodesMap;
    };
}

#endif //YPI_GUI_CONTAINER_GRAPH_HPP