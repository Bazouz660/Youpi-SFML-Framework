#include "TickBox.hpp"
#include "logger.hpp"

namespace ypi::gui {

    TickBox::TickBox()
    {
        m_shape.setSize({20, 20});
        m_shape.setOrigin(m_shape.getSize() / 2.f);

        m_tick.setSize({14, 14});
        m_tick.setOrigin(m_tick.getSize() / 2.f);
        m_tick.setFillColor(m_tickColor);

        setCallback([this]() {
            m_ticked = !m_ticked;
            notify(m_ticked);
        });
    }

    void TickBox::setTickColor(const sf::Color& color)
    {
        m_tickColor = color;
        m_tick.setFillColor(m_tickColor);
    }

    void TickBox::render(RenderTarget& target)
    {
        Button::render(target);
        if (m_ticked)
            target.draw(m_tick);
    }

    void TickBox::imp_setPosition()
    {
        m_shape.setPosition(m_absolutePosition);
        m_tick.setPosition(m_absolutePosition);
    }

    void TickBox::setTicked(bool ticked)
    {
        m_ticked = ticked;
    }

    bool TickBox::isTicked() const
    {
        return m_ticked;
    }

    void TickBox::setSize(const Vector2f& size)
    {
        m_shape.setSize(size);
        m_shape.setOrigin(size / 2.f);
        m_tick.setSize({size.x * 0.7f, size.y * 0.7f});
        m_tick.setOrigin(m_tick.getSize() / 2.f);
    }

    void TickBox::setTickTexture(const sf::Texture& texture)
    {
        m_tick.setTexture(&texture);
    }
}