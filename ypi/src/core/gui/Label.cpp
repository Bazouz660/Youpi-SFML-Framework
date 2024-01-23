/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** Label
*/

#include "Label.hpp"

namespace exng::gui
{
    Label::Label(const exng::Vector2f &position, const std::string &text, int size)
    {
        m_text.setPosition(position.x, position.y);
        m_text.setString(text);
        m_text.setCharacterSize(size);
    }

    void Label::render(Window &window)
    {
        window.draw(m_text);
    }

    void Label::update()
    {
    }

    void Label::handleEvent(const sf::Event &event)
    {
    }

    sf::Text &Label::getText()
    {
        return m_text;
    }

    void Label::setPosition(const sf::Vector2f &position)
    {
        m_text.setPosition(position);
        m_shape.setPosition(position);
    }
}