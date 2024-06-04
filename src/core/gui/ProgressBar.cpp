#include "ProgressBar.hpp"
#include "ResourceManager.hpp"

namespace ypi::gui
{
    ProgressBar::ProgressBar()
    {
        m_shape.setPosition(Vector2f(0, 0));
        m_shape.setSize(Vector2f(100, 10));
        m_bar.setPosition(Vector2f(0, 0));
        m_bar.setSize(Vector2f(100, 10));
        m_bar.setFillColor(sf::Color::Green);

        setPosition({0, 0});
    }

    float ProgressBar::getProgress() const
    {
        return m_progress;
    }

    void ProgressBar::setProgress(float progress)
    {
        m_progress = progress;
    }

    void ProgressBar::render(RenderTarget& target)
    {
        if (m_alignment == Alignment::HORIZONTAL) {
            m_bar.setSize(sf::Vector2f(m_shape.getSize().x * m_progress, m_shape.getSize().y));
            if (m_bar.getTexture())
                m_bar.setTextureRect(sf::IntRect(0, 0, m_bar.getTexture()->getSize().x * m_progress, m_bar.getTexture()->getSize().y));
            }
        else if (m_alignment == Alignment::VERTICAL) {
            m_bar.setSize(sf::Vector2f(m_shape.getSize().x, m_shape.getSize().y * m_progress));
            if (m_bar.getTexture())
                m_bar.setTextureRect(sf::IntRect(0, 0, m_bar.getTexture()->getSize().x, m_bar.getTexture()->getSize().y * m_progress));
        }
        target.draw(m_shape);
        target.draw(m_bar);
    }

    sf::RectangleShape& ProgressBar::getBar()
    {
        return m_bar;
    }

    void ProgressBar::imp_setPosition()
    {
        m_shape.setPosition(m_absolutePosition);
        m_bar.setPosition(m_shape.getPosition());
    }

    FloatRect ProgressBar::getGlobalBounds() const
    {
        FloatRect bounds;
        sf::FloatRect shapeBounds = m_shape.getGlobalBounds();

        bounds.left = shapeBounds.left;
        bounds.top = shapeBounds.top;
        bounds.width = shapeBounds.width;
        bounds.height = shapeBounds.height;

        return bounds;
    }

    void ProgressBar::setSize(const Vector2f& size)
    {
        m_shape.setSize(size);
        m_bar.setSize(size);
    }

    void ProgressBar::setAlignment(Alignment alignment)
    {
        m_alignment = alignment;
    }

    void ProgressBar::onNotify(float event)
    {
        event = std::clamp(event, 0.f, 1.f);
        setProgress(event);
    }
}
