#include "Button.hpp"
#include "ResourceManager.hpp"
#include "context.hpp"
#include "Label.hpp"

namespace ypi::gui
{
    Button::Button()
    {
        m_text = std::make_shared<Label>();

        m_stateCallbacks[State::Idle] = [](Button* button) {
            button->m_shape.setFillColor(sf::Color(200, 200, 200, 255));
            button->m_shape.setOutlineColor(sf::Color::Black);
            button->m_text->setFillColor(sf::Color(120, 120, 120, 255));
            button->m_text->setOutlineColor(sf::Color::White);
        };

        m_stateCallbacks[State::Hovered] = [](Button* button) {
            button->m_shape.setFillColor(sf::Color(150, 150, 150, 255));
            button->m_shape.setOutlineColor(sf::Color::Black);
            button->m_text->setFillColor(sf::Color(230, 230, 230, 255));
            button->m_text->setOutlineColor(sf::Color::White);
        };

        m_stateCallbacks[State::Selected] = [](Button* button) {
            button->m_shape.setFillColor(sf::Color(100, 100, 100, 255));
            button->m_shape.setOutlineColor(sf::Color::Black);
            button->m_text->setFillColor(sf::Color(150, 150, 150, 255));
            button->m_text->setOutlineColor(sf::Color::White);
        };

        setBackgroundSize({200, 50});
        m_text->setString("Button");
        m_text->setHorizontalAlignment(Label::AlignmentH::CENTER);
        m_text->setVerticalAlignment(Label::AlignmentV::CENTER);

        addChild(m_text);

        m_stateCallbacks[State::Idle](this);
    }

    FloatRect Button::getGlobalBounds() const
    {
        FloatRect shapeBounds = m_shape.getGlobalBounds();
        FloatRect textBounds = m_text->getGlobalBounds();
        // combine the bounds of the shape and the text
        return shapeBounds.combine(textBounds);
    }

    void Button::render(RenderTarget &window)
    {
        window.draw(m_shape);
        m_text->render(window);
    }

    void Button::setIdle()
    {
        if (m_state == State::Idle)
            return;
        m_state = State::Idle;
        if (m_stateCallbacks.contains(m_state))
            m_stateCallbacks[m_state](this);
    }

    void Button::setHovered()
    {
        if (m_state == State::Hovered)
            return;
        m_state = State::Hovered;
        if (m_stateCallbacks.contains(m_state))
            m_stateCallbacks[m_state](this);
    }

    void Button::setSelected()
    {
        if (m_state == State::Selected)
            return;
        m_state = State::Selected;
        if (m_stateCallbacks.contains(m_state))
            m_stateCallbacks[m_state](this);
    }

    void Button::handleHoverEvents(const sf::Event &event, const Vector2f& mousePos)
    {
        if (!Button::getGlobalBounds().contains(mousePos))
            return;
        if (event.type == sf::Event::MouseMoved) {
            if (m_state != State::Selected)
                setHovered();
        } else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                setSelected();
        } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            if (m_state == State::Selected) {
                m_callback();
                setIdle();
            } else {
                setHovered();
            }
        }
    }

    void Button::handleAnyEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        if (event.type == sf::Event::MouseMoved && !Button::getGlobalBounds().contains(mousePos)) {
            setIdle();
        }
    }

    void Button::setBackgroundSize(const Vector2f &size)
    {
        m_shape.setSize(size);
        m_shape.setOrigin(size.x / 2.f, size.y / 2.f);
    }

    void Button::imp_setPosition()
    {
        m_shape.setPosition(m_absolutePosition);
        m_text->setPosition(m_labelOffset);
    }

    void Button::setStateCallback(State state, std::function<void(Button* button)> callback)
    {
        m_stateCallbacks[state] = callback;
        if (m_state == state)
            callback(this);
    }

    void Button::setCallback(std::function<void()> callback)
    {
        m_callback = callback;
    }

    void Button::setBackgroundTexture(const sf::Texture& texture)
    {
        m_shape.setTexture(&texture);
    }

    void Button::setBackgroundColor(const sf::Color& color)
    {
        m_shape.setFillColor(color);
    }

    void Button::setBackgroundOutlineColor(const sf::Color& color)
    {
        m_shape.setOutlineColor(color);
    }

    void Button::setBackgroundOutlineThickness(float thickness)
    {
        m_shape.setOutlineThickness(thickness);
    }

    std::function<void(Button* button)> Button::getStateCallback(State state) const
    {
        return m_stateCallbacks.at(state);
    }

    void Button::setLabelOffset(const Vector2f& offset)
    {
        m_labelOffset = offset;
        m_text->setPosition(m_labelOffset);
    }
}
