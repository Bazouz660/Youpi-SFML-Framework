#ifndef YPI_GUI_TICKBOX_HPP
#define YPI_GUI_TICKBOX_HPP

#include "AComponent.hpp"
#include "Button.hpp"

namespace ypi::gui
{
    class TickBox : virtual public AComponent, protected Button, public Subject<bool>
    {
    protected:
        bool m_ticked = false;
        sf::RectangleShape m_tick;
        sf::Color m_tickColor = sf::Color::Green;

        virtual void imp_setPosition() override;

    public:
        TickBox();
        ~TickBox() override = default;

        bool isTicked() const;
        void setTicked(bool ticked);
        void setTickColor(const sf::Color& color);
        void setTickTexture(const sf::Texture& texture);

        void setSize(const Vector2f& size);

        virtual void render(RenderTarget& target) override;
    };
}

#endif // !YPI_GUI_TICKBOX_HPP