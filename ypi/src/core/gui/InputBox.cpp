#include "InputBox.hpp"
#include "../resource_manager/ResourceManager.hpp"

namespace exng::gui
{

    InputBox::InputBox(const sf::Vector2f &size, const sf::Vector2f &position, const std::string &text)
    {
        m_shape.setSize(size);
        m_shape.setOrigin(m_shape.getGlobalBounds().width / 2.0f, m_shape.getGlobalBounds().height / 4.0f);
        m_input.setOrigin(m_input.getGlobalBounds().width, m_input.getGlobalBounds().height / 2.0f);
        m_input.setFont(ResourceManager::getFont("font", "upheavtt"));
        m_input.setCharacterSize(20);
        m_input.setString(text);

        m_textureIdle = &ResourceManager::getTexture("gui", "gui_inputbox");
        m_textureSelected = &ResourceManager::getTexture("gui", "gui_inputbox_selected");
        setPosition(position);
    }

    void InputBox::render(Window &window)
    {
        if (m_state == State::Hovered)
            m_shape.setOutlineColor(sf::Color::Blue);
        else if (m_state == State::Selected)
            m_shape.setTexture(m_textureSelected);
        else
            m_shape.setTexture(m_textureIdle);

        window.draw(m_shape);
        window.draw(m_input);
    }

    void InputBox::update()
    {
    }

    void InputBox::handleEvent(const sf::Event &event)
    {
        // toggle the state of the input box
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (m_shape.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                if (m_state == State::Hovered) {
                    m_state = State::Selected;
                } else if (m_state == State::Selected) {
                    m_state = State::Hovered;
                }
            } else
                m_state = State::Idle;
        } else if (event.type == sf::Event::MouseMoved) {
            if (m_shape.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                if (m_state == State::Idle) {
                    m_state = State::Hovered;
                }
            } else if (m_state == State::Hovered) {
                m_state = State::Idle;
            }
        }

        // handle the input
        if (event.type == sf::Event::TextEntered && m_state == State::Selected)
        {
            if (event.text.unicode == 8 && m_inputString.getSize() > 0)
                m_inputString.erase(m_inputString.getSize() - 1, 1);
            else if (event.text.unicode >= 32 && event.text.unicode <= 126)
                m_inputString += static_cast<char>(event.text.unicode);
            m_input.setString(m_inputString);

            // check if the input is too long
            if (m_input.getGlobalBounds().width > m_shape.getGlobalBounds().width - 10) {
                m_inputString.erase(m_inputString.getSize() - 1, 1);
                m_input.setString(m_inputString);
            }
        } else if (event.type == sf::Event::KeyPressed && m_state == State::Selected) {
            if (event.key.control && event.key.code == sf::Keyboard::C) // Ctrl+C
            {
                sf::Clipboard::setString(m_inputString.toAnsiString());
            }
            else if (event.key.control && event.key.code == sf::Keyboard::V) // Ctrl+V
            {
                m_inputString = sf::Clipboard::getString();
                m_input.setString(m_inputString);
            }
        }
    }

    sf::Text &InputBox::getInput()
    {
        return m_input;
    }

    void InputBox::setPosition(const sf::Vector2f &pos)
    {
        m_shape.setPosition(pos);
        m_input.setPosition(m_shape.getGlobalBounds().left + 20, m_shape.getPosition().y);
    }

    std::string InputBox::getInputString() const
    {
        return m_inputString.toAnsiString();
    }
}
