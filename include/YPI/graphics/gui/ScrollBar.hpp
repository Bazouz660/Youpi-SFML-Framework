#pragma once

#include "Slider.hpp"

namespace ypi::gui
{
    class ScrollBar : public Slider
    {
    private:
        float m_scrollableRatio = 1.f;

    public:
        ScrollBar() = default;

        virtual void handleHoverEvents(const sf::Event& event, const Vector2f& mousePos) override;
        virtual void handleAnyEvents(const sf::Event& event, const Vector2f& mousePos) override;
        virtual void render(RenderTarget& target) override;

        void setScrollableRatio(float ratio);

    protected:
        void updateCartSize();
    };
}