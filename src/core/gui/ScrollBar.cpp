#include "ScrollBar.hpp"

namespace ypi::gui
{

    void ScrollBar::setScrollableRatio(float ratio)
    {
        m_scrollableRatio = std::clamp(ratio, 0.f, 1.f);
        m_delta = m_scrollableRatio;
        if (m_scrollableRatio == 1.f)
            notify(0.f);
        updateCartSize();
    }

    void ScrollBar::updateCartSize()
    {
        if (m_alignment == Alignment::HORIZONTAL) {
            m_cart.setSize(Vector2f(m_shape.getSize().x * m_scrollableRatio, m_cart.getSize().y));
        }
        else if (m_alignment == Alignment::VERTICAL) {
            m_cart.setSize(Vector2f(m_cart.getSize().x, m_shape.getSize().y * m_scrollableRatio));
        }

        m_cart.setOrigin(m_cart.getSize() / 2.f);
        Slider::updateCartPosition();
    }

    void ScrollBar::handleAnyEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        if (m_scrollableRatio == 1.f)
            return;
        Slider::handleAnyEvents(event, mousePos);
    }

    void ScrollBar::handleHoverEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        if (m_scrollableRatio == 1.f)
            return;
        Slider::handleHoverEvents(event, mousePos);
    }

    void ScrollBar::render(RenderTarget& target)
    {
        if (m_scrollableRatio == 1.f)
            return;
        Slider::render(target);
    }
}