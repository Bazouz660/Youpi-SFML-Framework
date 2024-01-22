#include "Image.hpp"

namespace exng::gui
{
    Image::Image(const sf::Texture& texture, const sf::Vector2f& position)
    {
        m_shape.setTexture(&texture);
        m_shape.setSize(sf::Vector2f(texture.getSize()));
        m_shape.setOrigin(sf::Vector2f(texture.getSize()) / 2.0f);
        setPosition(position);
    }

    void Image::render(Window& window)
    {
        window.draw(m_shape);
    }

    void Image::update()
    {
    }

    void Image::handleEvent(const sf::Event&)
    {
    }

    void Image::setPosition(const sf::Vector2f& position)
    {
        m_shape.setPosition(position);
    }
}
