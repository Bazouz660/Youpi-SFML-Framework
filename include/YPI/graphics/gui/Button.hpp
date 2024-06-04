#ifndef YPI_GUI_BUTTON_HPP
#define YPI_GUI_BUTTON_HPP

#include <functional>
#include "AComponent.hpp"
#include "Label.hpp"

namespace ypi::gui
{
    /**
     * @brief The Button class represents a graphical button component.
     *
     * It inherits from AComponent and provides functionality for rendering,
     * updating, and handling events for a button. It also contains a text
     * element and a callback function that is executed when the button is clicked.
     */
    class Button : virtual public AComponent {
    public:
        enum class State {
            Idle,
            Hovered,
            Selected
        };

    protected:
        std::shared_ptr<Label> m_text; /**< The text element of the button. */
        std::function<void()> m_callback = []() {}; /**< The callback function to be executed when the button is clicked. */
        State m_state = State::Idle; /**< The state of the button. */
        std::unordered_map<State, std::function<void(Button* button)>> m_stateCallbacks; /**< The callback functions for each state. */
        ypi::Vector2f m_labelOffset; /**< The offset of the label from the button's position. */

        void setIdle();
        void setHovered();
        void setSelected();

        virtual void imp_setPosition() override;
    public:
        /**
         * @brief Default constructor for Button.
         */
        Button();
        Button(const Button&) = delete;

        virtual ~Button() override = default;

        virtual void render(RenderTarget& target) override;
        virtual void handleHoverEvents(const sf::Event& event, const Vector2f& mousePos) override;
        virtual void handleAnyEvents(const sf::Event& event, const Vector2f& mousePos) override;
        virtual FloatRect getGlobalBounds() const override;

        virtual void setStateCallback(State state, std::function<void(Button* button)> callback);
        virtual std::function<void(Button* button)> getStateCallback(State state) const;
        virtual void setCallback(std::function<void()> callback);

        virtual void setBackgroundSize(const Vector2f& size);
        virtual void setBackgroundTexture(const sf::Texture& texture);
        virtual void setBackgroundColor(const sf::Color& color);
        virtual void setBackgroundOutlineColor(const sf::Color& color);
        virtual void setBackgroundOutlineThickness(float thickness);

        virtual void setLabelOffset(const Vector2f& offset);

        Label& getLabel() { return *m_text; }
        const Label& getLabel() const { return *m_text; }
    };
}

#endif //YPI_GUI_BUTTON_HPP