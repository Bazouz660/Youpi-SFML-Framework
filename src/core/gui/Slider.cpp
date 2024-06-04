#include "Slider.hpp"
#include "ResourceManager.hpp"
#include <cmath>
#include "context.hpp"

namespace ypi::gui
{
    Slider::Slider()
    {
        m_shape.setPosition(Vector2f(0, 0));
        m_shape.setSize(Vector2f(100, 10));
        m_shape.setOrigin(m_shape.getSize() / 2.f);
        m_cart.setPosition(Vector2f(0, 0));

        m_shape.setFillColor(sf::Color(75, 75, 75, 200));
        m_cart.setFillColor(sf::Color(200, 200, 200, 255));

        setCartSize(10);
        setProgress(0.f);
    }

    float Slider::getProgress() const
    {
        return m_progress;
    }

    void Slider::setProgress(float progress)
    {
        m_progress = std::clamp(progress, 0.f, 1.f);
        updateCartPosition();
    }

    void Slider::render(RenderTarget& target)
    {
        target.draw(m_shape);
        target.draw(m_cart);
    }

    void Slider::setCartSize(const unsigned int& size)
    {
        m_cart.setSize(Vector2f(size, size));
        m_cart.setOrigin(m_cart.getSize() / 2.f);

        updateCartPosition();
    }

    void Slider::updateCartPosition()
    {
        Vector2f cartHalfSize = m_cart.getSize() / 2.f;

        if (m_alignment == Alignment::HORIZONTAL) {
            float x = m_shape.getGlobalBounds().left + (m_shape.getGlobalBounds().width - m_cart.getSize().x) * m_progress + cartHalfSize.x;
            m_cart.setPosition(x, m_shape.getPosition().y);
        }
        else if (m_alignment == Alignment::VERTICAL) {
            float y = m_shape.getGlobalBounds().top + (m_shape.getGlobalBounds().height - m_cart.getSize().y) * m_progress + cartHalfSize.y;
            m_cart.setPosition(m_shape.getPosition().x, y);
        }
    }

    void Slider::handleScrolling(const sf::Event& event, const Vector2f& mousePos)
    {
        FloatRect bounds = getGlobalBounds();
        float scaler = m_alignment == Alignment::HORIZONTAL ? bounds.height : bounds.width;
        scaler = 5.f * (1.5f + 1.f / std::log(scaler + 2.f)); // Use an inverse logarithmic function to get a scaler that increases more for smaller sizes

        if (m_alignment == Alignment::VERTICAL) {
            bounds.width *= scaler;
            bounds.left -= (bounds.width - getGlobalBounds().width) / 2.f;
        } else {
            bounds.height *= scaler;
            bounds.top -= (bounds.height - getGlobalBounds().height) / 2.f;
        }

        if (bounds.contains(mousePos)) {

            if (m_state == State::Grabbed) {
                m_state = State::Hovered;
            }

            float sign = 1.f;
            if (m_alignment == Alignment::VERTICAL) {
                sign = -1.f;
            }

            m_progress += event.mouseWheelScroll.delta * m_delta * sign;

            m_progress = std::clamp(m_progress, 0.f, 1.f);

            if (!m_snapPoints.empty()) {
                float closestSnapPoint = *std::min_element(m_snapPoints.begin(), m_snapPoints.end(),
                    [this](float a, float b) {
                        return std::abs(m_progress - a) < std::abs(m_progress - b);
                    }
                );
                m_progress = closestSnapPoint;
            }

            updateCartPosition();
            notify(m_progress);
        }
    }

    void Slider::handleGrabbing(const sf::Event& event, const Vector2f& mousePos)
    {
        if (m_state == State::Grabbed) {
            Vector2f cartHalfSize = m_cart.getSize() / 2.f;
            Vector2f newPos = mousePos + m_grabOffset; // Use the stored relative position
            if (m_alignment == Alignment::HORIZONTAL) {
                m_progress = (newPos.x - m_shape.getGlobalBounds().left - cartHalfSize.x) / (m_shape.getGlobalBounds().width - m_cart.getSize().x);
            }
            else if (m_alignment == Alignment::VERTICAL) {
                m_progress = (newPos.y - m_shape.getGlobalBounds().top - cartHalfSize.y) / (m_shape.getGlobalBounds().height - m_cart.getSize().y);
            }
            m_progress = std::clamp(m_progress, 0.f, 1.f);
            if (!m_snapPoints.empty()) {
                float closestSnapPoint = *std::min_element(m_snapPoints.begin(), m_snapPoints.end(),
                    [this](float a, float b) {
                        return std::abs(m_progress - a) < std::abs(m_progress - b);
                    }
                );
                m_progress = closestSnapPoint;
            }
            updateCartPosition();
            notify(m_progress);
        }
    }

    void Slider::handleAnyEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        if (event.type == sf::Event::MouseWheelScrolled) {
            handleScrolling(event, mousePos);
        } else if (event.type == sf::Event::MouseMoved) {
            handleGrabbing(event, mousePos);
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (getGlobalBounds().contains(mousePos)) {
                m_state = State::Grabbed;
                m_grabOffset = m_cart.getPosition() - mousePos; // Store the relative position of the mouse to the cart
            }
        } else if (event.type == sf::Event::MouseButtonReleased) {
            m_state = State::Idle;
        }
    }

    sf::RectangleShape& Slider::getCart()
    {
        return m_cart;
    }

    void Slider::imp_setPosition()
    {
        m_shape.setPosition(m_absolutePosition);
        updateCartPosition();
    }

    FloatRect Slider::getGlobalBounds() const
    {
        FloatRect bounds = m_shape.getGlobalBounds();

        bounds = bounds.combine(m_cart.getGlobalBounds());

        return bounds;
    }

    void Slider::setSize(const Vector2f& size)
    {
        m_shape.setSize(size);
        m_shape.setOrigin(size / 2.f);
        updateCartPosition();
    }

    void Slider::setAlignment(Alignment alignment)
    {
        m_alignment = alignment;
        updateCartPosition();
    }

    void Slider::addSnapPoint(float snapPoint)
    {
        m_snapPoints.push_back(snapPoint);
    }

    void Slider::setSnapPoints(const std::vector<float>& snapPoints)
    {
        m_snapPoints = snapPoints;
    }

    void Slider::clearSnapPoints()
    {
        m_snapPoints.clear();
    }

    void Slider::setCartColor(const sf::Color& color)
    {
        m_cart.setFillColor(color);
    }

    void Slider::setCartOutlineColor(const sf::Color& color)
    {
        m_cart.setOutlineColor(color);
    }

    void Slider::setCartOutlineThickness(float thickness)
    {
        m_cart.setOutlineThickness(thickness);
    }

    void Slider::setCartTexture(const sf::Texture& texture)
    {
        m_cart.setTexture(&texture);
    }

    void Slider::setBarColor(const sf::Color& color)
    {
        m_shape.setFillColor(color);
    }

    void Slider::setBarOutlineColor(const sf::Color& color)
    {
        m_shape.setOutlineColor(color);
    }

    void Slider::setBarOutlineThickness(float thickness)
    {
        m_shape.setOutlineThickness(thickness);
    }

    void Slider::setBarTexture(const sf::Texture& texture)
    {
        m_shape.setTexture(&texture);
    }
}
