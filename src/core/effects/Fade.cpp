/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** Fade
*/

#include "Fade.hpp"

#include "logger.hpp"
#include "math.hpp"

namespace ypi::effect {

    Fade::Fade(float fadeInDuration, float fadeOutDuration)
    {
        m_alpha = 0;
        m_state = State::FadeIn;
        m_timer = 0;
        m_fadeInDuration = fadeInDuration;
        m_fadeOutDuration = fadeOutDuration;
    }

    void Fade::update(float dt)
    {
        m_timer += dt;
        if (m_state == State::FadeIn) {
            float progress = m_timer / m_fadeInDuration;
            m_alpha = -(std::cos(PI * progress) / 2) + 0.5; // Interpolation sinusoïdale de 0 à 1
            if (m_timer >= m_fadeInDuration) {
                m_alpha = 1; // Assurez-vous que m_alpha est exactement 1 à la fin du FadeIn
                m_state = State::FadeOut;
                m_timer = 0; // Réinitialisez le timer pour la phase de fondu sortant
            }
        } else if (m_state == State::FadeOut) {
            float progress = m_timer / m_fadeOutDuration;
            m_alpha = -(std::cos(PI * (1 - progress)) / 2) + 0.5; // Interpolation sinusoïdale de 1 à 0
            if (m_timer >= m_fadeOutDuration) {
                m_alpha = 0; // Assurez-vous que m_alpha est exactement 0 à la fin du FadeOut
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