#include "AComponent.hpp"

#pragma once

namespace exng::gui
{
    class Image : public AComponent
    {
    public:
        Image(const sf::Texture&, const sf::Vector2f& = sf::Vector2f(0, 0));
        ~Image() = default;

        void render(Window&) override;
        void update() override;
        void handleEvent(const sf::Event&) override;
        void setPosition(const sf::Vector2f&) override;
    };
}
