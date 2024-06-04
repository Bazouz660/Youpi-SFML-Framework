/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** Label
*/

#include "Label.hpp"

namespace ypi::gui
{
    void Label::render(RenderTarget &target)
    {
        target.draw(m_text);
    }

    void Label::imp_setPosition()
    {
        m_text.setPosition(m_absolutePosition);
        m_shape.setPosition(m_absolutePosition);
    }

    FloatRect Label::getGlobalBounds() const
    {
        return m_text.getGlobalBounds();
    }

    void Label::onNotify(const std::string& event)
    {
        m_text.setString(event);
        align();
    }

    void Label::setString(const sf::String& text)
    {
        m_text.setString(text);
        align();
    }

    void Label::setFont(const sf::Font& font)
    {
        m_text.setFont(font);
        align();
    }

    void Label::setCharacterSize(unsigned int size)
    {
        m_text.setCharacterSize(size);
        align();
    }

    void Label::setFillColor(const sf::Color& color)
    {
        m_text.setFillColor(color);
    }

    void Label::setOutlineColor(const sf::Color& color)
    {
        m_text.setOutlineColor(color);
    }

    void Label::setOutlineThickness(float thickness)
    {
        m_text.setOutlineThickness(thickness);
        align();
    }

    void Label::setHorizontalAlignment(AlignmentH alignment)
    {
        m_alignmentH = alignment;
        align();
    }

    void Label::setVerticalAlignment(AlignmentV alignment)
    {
        m_alignmentV = alignment;
        align();
    }

    void Label::setStyle(sf::Text::Style style)
    {
        m_text.setStyle(style);
        align();
    }

    void Label::align()
    {
        if (m_alignmentH == AlignmentH::CENTER) {
            m_text.setOrigin(m_text.getGlobalBounds().width / 2.f, m_text.getOrigin().y);
        } else if (m_alignmentH == AlignmentH::LEFT) {
            m_text.setOrigin(0, m_text.getOrigin().y);
        } else if (m_alignmentH == AlignmentH::RIGHT) {
            m_text.setOrigin(m_text.getGlobalBounds().width, m_text.getOrigin().y);
        }

        if (m_alignmentV == AlignmentV::CENTER) {
            m_text.setOrigin(m_text.getOrigin().x, m_text.getGlobalBounds().height / 2.f);
        } else if (m_alignmentV == AlignmentV::TOP) {
            m_text.setOrigin(m_text.getOrigin().x, m_text.getGlobalBounds().height);
        } else if (m_alignmentV == AlignmentV::BOTTOM) {
            m_text.setOrigin(m_text.getOrigin().x, 0);
        }
    }

    sf::String Label::getString() const
    {
        return m_text.getString();
    }

    const sf::Font& Label::getFont() const
    {
        return *m_text.getFont();
    }

    unsigned int Label::getCharacterSize() const
    {
        return m_text.getCharacterSize();
    }

    const sf::Color& Label::getFillColor() const
    {
        return m_text.getFillColor();
    }

    const sf::Color& Label::getOutlineColor() const
    {
        return m_text.getOutlineColor();
    }

    float Label::getOutlineThickness() const
    {
        return m_text.getOutlineThickness();
    }

    Label::AlignmentH Label::getHorizontalAlignment() const
    {
        return m_alignmentH;
    }

    Label::AlignmentV Label::getVerticalAlignment() const
    {
        return m_alignmentV;
    }

    Label::Label(const Label& label)
    {
        m_text = label.m_text;
        m_alignmentH = label.m_alignmentH;
        m_alignmentV = label.m_alignmentV;
    }

    Vector2f Label::findCharacterPos(std::size_t index) const
    {
        return m_text.findCharacterPos(index);
    }

    Vector2f Label::getOrigin() const
    {
        return m_text.getOrigin();
    }

}