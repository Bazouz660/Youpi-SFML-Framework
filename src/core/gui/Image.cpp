#include "Image.hpp"

namespace ypi::gui
{
    Image::Image(const sf::Texture& texture, const Vector2f& position)
    {
        m_shape.setTexture(&texture);
        m_shape.setSize({(float)texture.getSize().x, (float)texture.getSize().y});
        m_shape.setOrigin({(float)texture.getSize().x / 2, (float)texture.getSize().y / 2});
        setPosition(position);
    }

    void Image::render(RenderTarget& target)
    {
        target.draw(m_shape);
    }

    void Image::imp_setPosition()
    {
        m_shape.setPosition(m_absolutePosition);
    }

    FloatRect Image::getGlobalBounds() const
    {
        FloatRect bounds;
        sf::FloatRect shapeBounds = m_shape.getGlobalBounds();

        bounds.left = shapeBounds.left;
        bounds.top = shapeBounds.top;
        bounds.width = shapeBounds.width;
        bounds.height = shapeBounds.height;

        return bounds;
    }

}
