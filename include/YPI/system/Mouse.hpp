#pragma once

#include "../../lib_headers/common.hpp"
#include "../math/math.hpp"
#include <chrono>

namespace ypi {

    class MouseClickHandler {
    public:
        MouseClickHandler() {
            m_lastClickTime = std::chrono::high_resolution_clock::now();
        }

        bool isDoubleClick(const sf::Event& event
            , std::chrono::milliseconds maxTimeBetweenClicks = std::chrono::milliseconds(500)
            , float maxDistanceBetweenClicks = 20.0f) {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (!m_lastClick) {
                    m_lastClick = true;
                    m_lastClickPos = sf::Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y);
                }
                auto currentTime = std::chrono::high_resolution_clock::now();
                auto timeSinceLastClick = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_lastClickTime);
                if (timeSinceLastClick < maxTimeBetweenClicks
                    && math::dist(m_lastClickPos, sf::Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y)) < maxDistanceBetweenClicks) {
                    m_lastClickTime = currentTime;
                    return true;
                }
                m_lastClickTime = currentTime;
                m_lastClickPos = sf::Vector2f((float)event.mouseButton.x, (float)event.mouseButton.y);
            }
            return false;
        }

    private:
        std::chrono::high_resolution_clock::time_point m_lastClickTime;
        sf::Vector2f m_lastClickPos = { 0, 0 };
        bool m_lastClick = false;

    };

}