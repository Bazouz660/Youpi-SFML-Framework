/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** Container
*/


#ifndef EXNG_GUI_CONTAINER_HPP
#define EXNG_GUI_CONTAINER_HPP

#include "AComponent.hpp"

#include <vector>
#include <utility>

namespace exng::gui
{
    /**
     * @brief The Container class represents a GUI container that can hold multiple components.
     *
     * It inherits from AComponent and provides functionality to add, retrieve, handle events, update, and render components.
     */
    class Container : public AComponent {
    protected:
        std::vector<std::pair<std::shared_ptr<AComponent>, bool>> m_components; /**< The vector of components contained in the container. */

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
        virtual void addComponent(std::shared_ptr<AComponent> component);

        /**
         * @brief Retrieves a component of type T at the specified index.
         *
         * @tparam T The type of the component to retrieve.
         * @param index The index of the component in the container.
         * @return std::shared_ptr<T> The shared pointer to the retrieved component.
         */
        template <typename T>
        std::shared_ptr<T> getComponent(int index)
        {
            return std::dynamic_pointer_cast<T>(m_components.at(index).first);
        };

        /**
         * @brief Handles an SFML event.
         *
         * @param event The SFML event to handle.
         */
        virtual void handleEvent(const sf::Event& event) override;

        /**
         * @brief Updates the container and its components.
         */
        virtual void update() override;

        /**
         * @brief Renders the container and its components to a window.
         *
         * @param window The window to render to.
         */
        virtual void render(Window& window) override;

        /**
         * @brief Sets the position of the container.
         *
         * @param position The position to set.
         */
        virtual void setPosition(const sf::Vector2f& position) override;

        bool isComponentEnabled(int index) const;
        void toggleComponent(int index, bool enabled);
    };
}

#endif //EXNG_GUI_CONTAINER_HPP