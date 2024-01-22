#include "Button.hpp"
#include "core/resource_manager/ResourceManager.hpp"

namespace exng::gui
{
    Button::Button(const sf::Vector2f &size, const sf::Vector2f &position, const std::string &text, std::function<void()> callback) : m_callback(callback)
    {
        m_shape.setSize(size);

        m_text.setString(text);
        m_text.setFont(exng::ResourceManager::getFont("font", "upheavtt"));
        m_shape.setOrigin(m_shape.getGlobalBounds().width / 2.0f, m_shape.getGlobalBounds().height / 4.0f);
        m_text.setOrigin(m_text.getGlobalBounds().width / 2.0f, m_text.getGlobalBounds().height / 2.0f);

        m_textureIdle = &exng::ResourceManager::getTexture("gui", "gui_button");
        m_textureHover = &exng::ResourceManager::getTexture("gui", "gui_button_hover");
        m_texturePressed = &exng::ResourceManager::getTexture("gui", "gui_button_pressed");
        setPosition(position);
    }

    sf::Text &Button::getText()
    {
        return m_text;
    }

    void Button::render(Window &window)
    {
        if (m_state == State::Hovered)
        {
            if (m_textureHover->getSize().x > 0 && m_textureHover->getSize().y > 0)
                m_shape.setTexture(m_textureHover);
            else
                m_shape.setFillColor(sf::Color(100, 100, 100, 255));
        }
        else if (m_state == State::Selected)
        {
            if (m_texturePressed->getSize().x > 0 && m_texturePressed->getSize().y > 0)
                m_shape.setTexture(m_texturePressed);
            else
                m_shape.setFillColor(sf::Color(100, 100, 100, 255));
        }
        else
        {
            if (m_textureIdle->getSize().x > 0 && m_textureIdle->getSize().y > 0)
                m_shape.setTexture(m_textureIdle);
            else
                m_shape.setFillColor(sf::Color::Black);
        }
        window.draw(m_shape);
        window.draw(m_text);
    }

    void Button::update()
    {
    }

    void Button::handleEvent(const sf::Event &event)
    {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            if (m_shape.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && m_callback)
            {
                if (m_state == State::Hovered)
                {
                    m_state = State::Selected;
                }
                else if (m_state == State::Selected)
                {
                    m_state = State::Hovered;
                }
                m_callback();
            }
            else
                m_state = State::Idle;
        }
        else if (event.type == sf::Event::MouseMoved)
        {
            if (m_shape.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
            {
                if (m_state == State::Idle)
                {
                    m_state = State::Hovered;
                }
            }
            else if (m_state == State::Hovered)
            {
                m_state = State::Idle;
            }
        }
    }

    void Button::setPosition(const sf::Vector2f &pos)
    {
        m_shape.setPosition(pos);
        m_text.setPosition(m_shape.getPosition());
    }
}
