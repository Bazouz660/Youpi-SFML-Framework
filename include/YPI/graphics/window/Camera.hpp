#ifndef YPI_CAMERA_HPP_
#define YPI_CAMERA_HPP_

#include "../../../lib_headers/common.hpp"
#include "../../math/Vector2.hpp"
#include "../../math/math.hpp"
#include "../gui/AComponent.hpp"

namespace ypi::gui {

    class Camera : public sf::View, public AComponent
    {
    public:
        enum class Mode
        {
            Fixed,
            Free,
            Follow
        };

        Camera() = default;
        ~Camera() = default;

        void setMode(Mode mode);
        void setTarget(const Vector2f& target);
        void setSpeed(float speed);
        void setZoom(float zoom);

        void setSize(const Vector2f& size);

        void update(float dt);
        void handleHoverEvents(const sf::Event& event, const Vector2f& mousePos) override;
        void handleAnyEvents(const sf::Event& event, const Vector2f& mousePos) override;
        void render(RenderTarget& target) override {}
        FloatRect getGlobalBounds() const override;

        Mode getMode() const;
        Vector2f getTarget() const;
        float getSpeed() const;
        float getZoom() const;

        void setControls(const sf::Keyboard::Key& up,
                         const sf::Keyboard::Key& down,
                         const sf::Keyboard::Key& left,
                         const sf::Keyboard::Key& right);

        void setEasingFunction(math::EasingFunction easingFunction);

        // applies the camera to the render target
        void start(sf::RenderTexture& renderTarget);

        // resets the camera to the default view
        void stop();

        Vector2f getMousePostion(const sf::RenderWindow& window) const;

    private:
        void updateFixed(float dt);
        void updateFree(float dt);
        void updateFollow(float dt);

        void updatePosition(float dt);

    private:
        Mode m_mode = Mode::Fixed;
        Vector2f m_target;
        Vector2f m_velocity;
        Vector2f m_acceleration;

        Vector2f m_baseSize;
        float m_speed = 1000.f;
        float m_breakSpeed = 10.f;
        float m_zoom = 1.f;
        float m_zoomSpeed = 10.f;
        float m_zoomMin = 0.1f;
        float m_zoomMax = 10.f;
        float m_zoomGoal = 1.f;
        math::EasingFunction m_easingFunction = math::EasingFunction::Linear;

        sf::RenderTexture* m_renderTarget = nullptr;

        bool m_dragging = false;
        Vector2f m_lastMousePos;
        sf::Keyboard::Key m_up = sf::Keyboard::Z;
        sf::Keyboard::Key m_down = sf::Keyboard::S;
        sf::Keyboard::Key m_left = sf::Keyboard::Q;
        sf::Keyboard::Key m_right = sf::Keyboard::D;

        bool m_moveUp = false;
        bool m_moveDown = false;
        bool m_moveLeft = false;
        bool m_moveRight = false;
    };
} // namespace ypi

#endif // YPI_WINDOW_HPP_