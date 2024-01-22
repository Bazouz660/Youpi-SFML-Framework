#include "ProgressBar.hpp"
#include "ResourceManager.hpp"

namespace exng::gui
{
    ProgressBar::ProgressBar(const sf::Vector2f& size, const sf::Vector2f& position)
    {
        m_shape.setPosition(position);
        m_shape.setSize(size);
        m_bar.setPosition(position);
        m_bar.setSize(size);
    }

    float ProgressBar::getProgress() const
    {
        return m_progress;
    }

    void ProgressBar::setProgress(float progress)
    {
        m_progress = progress;
    }

    void ProgressBar::render(Window& window)
    {
        m_bar.setSize(sf::Vector2f(m_shape.getSize().x * m_progress, m_shape.getSize().y));
        if (m_bar.getTexture())
            m_bar.setTextureRect(sf::IntRect(0, 0, m_bar.getTexture()->getSize().x * m_progress, m_bar.getTexture()->getSize().y));
        window.draw(m_shape);
        window.draw(m_bar);
    }

    void ProgressBar::update()
    {
    }

    void ProgressBar::handleEvent(const sf::Event& event)
    {
    }

    sf::RectangleShape& ProgressBar::getBar()
    {
        return m_bar;
    }

    void ProgressBar::setPosition(const sf::Vector2f& position)
    {
        m_shape.setPosition(position);
        m_bar.setPosition(position);
    }
}
