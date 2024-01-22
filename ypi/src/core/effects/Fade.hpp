/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** Fade
*/

#ifndef EXNG_EFFECTS_FADE_HPP
#define EXNG_EFFECTS_FADE_HPP

#include "ypi/include/common.hpp"
#include "ypi/include/sfml.hpp"

namespace exng::effect {

    class Fade {
        public:
            Fade() = default;
            Fade(float fadeInDuration, float fadeOutDuration);
            ~Fade() = default;

            void update(float dt);
            float getAlpha() const;

            void reset();

            bool isFinished() const;

        private:
            float m_timer = 0;
            float m_fadeInDuration = 0;
            float m_fadeOutDuration = 0;
            float m_alpha = 0;

            enum class State {
                FadeIn,
                FadeOut,
                Finished
            };

            State m_state = State::FadeIn;
    };

}

#endif // !EXNG_EFFECTS_FADE_HPP