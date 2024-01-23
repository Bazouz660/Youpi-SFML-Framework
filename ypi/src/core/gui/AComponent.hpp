#include "ypi/lib_headers/sfml.hpp"
#include "../window/Window.hpp"

#ifndef EXNG_GUI_ACOMPONENT_HPP
#define EXNG_GUI_ACOMPONENT_HPP

namespace exng::gui
{
    /**
     * @brief The base class for GUI components.
     *
     * This class provides a common interface and functionality for GUI components.
     * It contains a protected member variable `m_shape` of type `sf::RectangleShape`
     * which represents the shape of the component.
     */
    class AComponent {
    protected:
        sf::RectangleShape m_shape;

    public:
        /**
         * @brief Default constructor.
         */
        AComponent() = default;

        /**
         * @brief Virtual destructor.
         */
        virtual ~AComponent() = default;

        /**
         * @brief Get the shape of the component.
         *
         * @return A reference to the `sf::RectangleShape` representing the shape of the component.
         */
        virtual sf::RectangleShape& getShape() { return m_shape; };

        /**
         * @brief Render the component on a window.
         *
         * @param window The window on which to render the component.
         */
        virtual void render(Window&) = 0;

        /**
         * @brief Update the component.
         *
         *
         * This function is called to update the state of the component.
         */
        virtual void update() = 0;

        /**
         * @brief Handle an event.
         *
         * This function is called to handle an event related to the component.
         *
         * @param event The event to handle.
         */
        virtual void handleEvent(const sf::Event&) = 0;

        /**
         * @brief Set the position of the component.
         *
         * @param pos The new position of the component.
         */
        virtual void setPosition(const sf::Vector2f& pos) = 0;
    };
}

#endif //EXNG_GUI_ACOMPONENT_HPP