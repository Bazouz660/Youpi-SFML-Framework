#include "Camera.hpp"

namespace ypi::gui {

    void Camera::setMode(Mode mode)
    {
        m_mode = mode;
    }

    void Camera::setTarget(const Vector2f& target)
    {
        m_target = target;
    }

    void Camera::setSpeed(float speed)
    {
        m_speed = speed;
    }

    void Camera::setZoom(float zoom)
    {
        m_zoomGoal = zoom;
    }

    void Camera::update(float dt)
    {
        switch (m_mode) {
            case Mode::Fixed:
                updateFixed(dt);
                break;
            case Mode::Free:
                updateFree(dt);
                break;
            case Mode::Follow:
                updateFollow(dt);
                break;
        }
    }

    void Camera::handleAnyEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        if (m_mode == Mode::Free) {

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == m_up) {
                    m_moveUp = false;
                } else if (event.key.code == m_down) {
                    m_moveDown = false;
                } else if (event.key.code == m_left) {
                    m_moveLeft = false;
                } else if (event.key.code == m_right) {
                    m_moveRight = false;
                }
            }
        }
    }

    void Camera::handleHoverEvents(const sf::Event& event, const Vector2f& mousePos)
    {
        if (event.type == sf::Event::MouseWheelScrolled && m_mode == Mode::Free) {
            float zoom = m_zoom;
            if (event.mouseWheelScroll.delta < 0) {
                zoom *= 1.1f;
            } else if (event.mouseWheelScroll.delta > 0) {
                zoom *= 0.9f;
            }
            if (zoom != m_zoom)
                setZoom(zoom);
        }

        if (m_mode == Mode::Free) {
            Vector2f mousePosGlobal = Vector2i(sf::Mouse::getPosition()).to<float>();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == m_up) {
                    m_moveUp = true;
                } else if (event.key.code == m_down) {
                    m_moveDown = true;
                } else if (event.key.code == m_left) {
                    m_moveLeft = true;
                } else if (event.key.code == m_right) {
                    m_moveRight = true;
                }
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == m_up) {
                    m_moveUp = false;
                } else if (event.key.code == m_down) {
                    m_moveDown = false;
                } else if (event.key.code == m_left) {
                    m_moveLeft = false;
                } else if (event.key.code == m_right) {
                    m_moveRight = false;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle) {
                m_dragging = true;
                m_lastMousePos = mousePosGlobal;
            } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Middle) {
                m_dragging = false;
            }

            if (m_dragging) {
                if (event.type == sf::Event::MouseMoved) {
                    Vector2f delta = m_lastMousePos - mousePosGlobal;
                    sf::View::move(delta * m_zoom);
                }
                m_lastMousePos = mousePosGlobal;
            }
        }
    }

    Camera::Mode Camera::getMode() const
    {
        return m_mode;
    }

    Vector2f Camera::getTarget() const
    {
        return m_target;
    }

    float Camera::getSpeed() const
    {
        return m_speed;
    }

    float Camera::getZoom() const
    {
        return m_zoom;
    }

    void Camera::updateFixed(float dt)
    {
        sf::View::setCenter(m_target);
    }

    void Camera::setControls(const sf::Keyboard::Key& up,
                             const sf::Keyboard::Key& down,
                             const sf::Keyboard::Key& left,
                             const sf::Keyboard::Key& right)
    {
        m_up = up;
        m_down = down;
        m_left = left;
        m_right = right;
    }

    void Camera::updatePosition(float dt)
    {
        m_velocity += m_acceleration * dt;
        m_velocity *= math::lerp(1.f, 0.f, dt * m_breakSpeed);
        sf::View::move(m_velocity * dt);
        m_acceleration = Vector2f(0.f, 0.f);
    }

    void Camera::updateFree(float dt)
    {
        if (m_moveUp) {
            m_acceleration.y = -1.f;
        }
        if (m_moveDown) {
            m_acceleration.y = 1.f;
        }
        if (m_moveLeft) {
            m_acceleration.x = -1.f;
        }
        if (m_moveRight) {
            m_acceleration.x = 1.f;
        }

        m_acceleration = math::normalize(m_acceleration) * m_speed * m_breakSpeed * std::clamp(0.5f, 1.f, m_zoom);

        updatePosition(dt);

        // update zoom, the easing function is used to smooth the zoom (the easing function takes one parameter t)
        if (m_zoom != m_zoomGoal) {
            m_zoom = math::lerp(m_zoom, m_zoomGoal, m_zoomSpeed * dt);
            sf::View::setSize(m_baseSize * m_zoom);
        }
    }

    void Camera::setEasingFunction(math::EasingFunction easingFunction)
    {
        m_easingFunction = easingFunction;
    }

    void Camera::updateFollow(float dt)
    {
        Vector2f direction = m_target - sf::View::getCenter();
        float distance = math::length(direction);
        if (distance > 1.f) {
            direction = math::normalize(direction);
            m_velocity = direction * m_speed;
        } else {
            m_velocity = Vector2f(0.f, 0.f);
        }

        updatePosition(dt);
    }

    void Camera::setSize(const Vector2f& size)
    {
        m_baseSize = size;
        sf::View::setCenter(size / 2.f);
        sf::View::setSize(size);
    }

    void Camera::start(sf::RenderTexture& renderTexture)
    {
        m_renderTarget = &renderTexture;
        m_renderTarget->setView(*this);
    }

    void Camera::stop()
    {
        if (m_renderTarget) {
            m_renderTarget->setView(m_renderTarget->getDefaultView());
        }
    }

    Vector2f Camera::getMousePostion(const sf::RenderWindow& window) const
    {
        return window.mapPixelToCoords(sf::Mouse::getPosition(window), *this);
    }

    // returns the global world bounds of the camera
    // the Viewport is a rectangle, expressed as a factor of the target size
    FloatRect Camera::getGlobalBounds() const
    {
        FloatRect bounds;
        bounds.left = 0;
        bounds.top = 0;
        bounds.width = sf::View::getSize().x;
        bounds.height = sf::View::getSize().y;
        bounds.width *= 1.f / m_zoom;
        bounds.height *= 1.f / m_zoom;
        return bounds;
    }
}