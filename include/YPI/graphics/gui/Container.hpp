/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** Container
*/


#ifndef YPI_GUI_CONTAINER_HPP
#define YPI_GUI_CONTAINER_HPP

#include "AComponent.hpp"

#include <vector>
#include <utility>

namespace ypi::gui
{
    /**
     * @brief The Container class represents a GUI container that can hold multiple components.
     *
     * It inherits from AComponent and provides functionality to add, retrieve, handle events, update, and render components.
     */
    class Container : public AComponent {
    protected:
        struct Component {
            std::shared_ptr<AComponent> component;
            bool enabled;
            int zIndex = 0;
        };

        std::unordered_map<std::string, std::pair<std::shared_ptr<AComponent>, bool>> m_components; /**< The vector of components contained in the container. */
        std::vector<Component> m_orderedComponents;

        void sortComponents();

    public:
        /**
         * @brief Default constructor for the Container class.
         */
        Container() = default;

        /**
         * @brief Default destructor for the Container class.
         */
        virtual ~Container() = default;

        /**
         * @brief Adds a component to the container.
         *
         * @param component The component to be added.
         */
        virtual void addComponent(const std::string& key, const_sptr_ref<AComponent> component, int zIndex = 0);

        /**
         * @brief Removes a component from the container.
         *
         * @param key The key of the component to remove.
         */
        virtual void removeComponent(const std::string& key);

        /**
         * @brief Retrieves a component of type T at the specified index.
         *
         * @tparam T The type of the component to retrieve.
         * @param index The index of the component in the container.
         * @return std::shared_ptr<T> The shared pointer to the retrieved component.
         */
        template <typename T>
        std::shared_ptr<T> getComponent(const std::string& key)
        {
            try {
                return std::dynamic_pointer_cast<T>(m_components.at(key).first);
            } catch (const std::out_of_range& e) {
                throw std::out_of_range("Component not found");
            }
        };

        /**
         * @brief Handles an SFML event.
         *
         * @param event The SFML event to handle.
         */
        virtual void handleAnyEvents(const sf::Event& event, const Vector2f& mousePos) override;

        virtual void handleHoverEvents(const sf::Event& event, const Vector2f& mousePos) override;

        /**
         * @brief Renders the container and its components to a window.
         *
         * @param window The window to render to.
         */
        virtual void render(RenderTarget& target) override;
        void render(RenderTarget& target, const sf::View& view);

        virtual FloatRect getGlobalBounds() const override;

        bool isComponentEnabled(const std::string& key) const;
        void toggleComponent(const std::string& key, bool enabled);
        void toggleComponent(const std::string& key);
        void clear();
    };
}

#endif //YPI_GUI_CONTAINER_HPP