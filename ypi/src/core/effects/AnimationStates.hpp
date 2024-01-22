/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** AnimationStates
*/

#ifndef EXNG_ANIMATIONSTATES_HPP
#define EXNG_ANIMATIONSTATES_HPP

#include "core/effects/SpriteSheet.hpp"
#include <map>

namespace exng::effect {

    class AnimationStates {
        public:
            AnimationStates() = default;
            ~AnimationStates() = default;

            void addState(const std::string &name, const SpriteSheet &spriteSheet, bool shouldReverse);
            void removeState(const std::string &name);
            void setState(const std::string &name, bool skip = false);
            void update(float dt);

            const SpriteSheet &getCurrentState() const;
            std::string getCurrentStateName() const;

        private:
            std::unordered_map<std::string, std::pair<SpriteSheet, bool>> m_states;
            std::string m_currentStateName = "";
            bool m_switchingState = false;
            bool m_reversing = false;
            std::string m_nextStateName = "";
            std::string m_previousStateName = "";
    };

}

#endif // !EXNG_ANIMATIONSTATES_HPP