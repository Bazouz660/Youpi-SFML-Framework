#include "Panel.hpp"

namespace ypi::gui
{
    Panel::Panel()
        : AComponent()
    {
        m_shape.setSize(Vector2f(0, 0));
        m_shape.setFillColor(sf::Color::Transparent);
        m_shape.setOrigin(0, 0);
    }

    void Panel::render(RenderTarget& target)
    {
        target.draw(m_shape);
    }

    void Panel::imp_setPosition()
    {
        m_shape.setPosition(m_absolutePosition);
    }
}