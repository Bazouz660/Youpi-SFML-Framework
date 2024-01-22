#include <functional>
#include "AComponent.hpp"

#ifndef EXNG_GUI_BUTTON_HPP
#define EXNG_GUI_BUTTON_HPP

namespace exng::gui
{
    /**
     * @brief The Button class represents a graphical button component.
     *
     * It inherits from AComponent and provides functionality for rendering,
     * updating, and handling events for a button. It also contains a text
     * element and a callback function that is executed when the button is clicked.
     */
    class Button : public AComponent {
    private:
        sf::Text m_text; /**< The text element of the button. */
        std::function<void()> m_callback; /**< The callback function to be executed when the button is clicked. */
        enum class State {
            Idle,
            Hovered,
            Selected
        } m_state = State::Idle; /**< The state of the button. */
        sf::Texture* m_textureIdle;
        sf::Texture* m_textureHover;
        sf::Texture* m_texturePressed;

    public:
        /**
         * @brief Default constructor for Button.
         */
        Button() = default;

        /**
         * @brief Constructor for Button.
         * @param size The size of the button.
         * @param position The position of the button.
         * @param text The text to be displayed on the button.
         * @param callback The callback function to be executed when the button is clicked.
         */
        Button(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text, std::function<void()> callback);

        /**
         * @brief Destructor for Button.
         */
        virtual ~Button() override = default;

        /**
         * @brief Get the text element of the button.
         * @return A reference to the text element.
         */
        virtual sf::Text& getText();

        /**
         * @brief Render the button on a window.
         * @param window The window to render the button on.
         */
        virtual void render(Window& window) override;

        /**
         * @brief Update the button.
         */
        virtual void update() override;

        /**
         * @brief Handle an event for the button.
         * @param event The event to handle.
         */
        virtual void handleEvent(const sf::Event& event) override;

        /**
         * @brief Set the position of the button.
         * @param pos The position to set.
         */
        virtual void setPosition(const sf::Vector2f& pos) override;
    };
}

#endif //EXNG_GUI_BUTTON_HPP