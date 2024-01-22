/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** Fade
*/

#include "Fade.hpp"

#include "helper/info/logger.hpp"
#include "helper/math/math.hpp"

namespace exng::effect {

    Fade::Fade(float fadeInDuration, float fadeOutDuration)
    {
        m_timer = 0;
        m_fadeInDuration = fadeInDuration;
        m_fadeOutDuration = fadeOutDuration;
    }

    void Fade::update(float dt)
    {
        m_timer += dt;
        if (m_state == State::FadeIn) {
            float progress = m_timer / m_fadeInDuration;
            m_alpha = 0.5f * (1 - std::cos(progress * PI));
            if (m_timer >= m_fadeInDuration) {
                m_state = State::FadeOut;
            }
        } else if (m_state == State::FadeOut) {
            float progress = (m_timer - m_fadeInDuration) / m_fadeOutDuration;
            m_alpha = 0.5f * (1 + std::cos(progress * PI));
            if (m_timer >= m_fadeInDuration + m_fadeOutDuration) {
                m_timer = 0;
                m_alpha = 0;
                m_state = State::Finished;
            }
        }
    }

    void Fade::reset()
    {
        m_timer = 0;
    }

    float Fade::getAlpha() const
    {
        return std::clamp(m_alpha, 0.f, 1.f);
    }

    bool Fade::isFinished() const
    {
        return m_timer == 0;
    }

}