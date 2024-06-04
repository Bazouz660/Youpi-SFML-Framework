/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** AnimationStates
*/

#include "AnimationStates.hpp"

#include "logger.hpp"

namespace ypi::effect {

    void AnimationStates::addState(const std::string &name, const SpriteSheet &spriteSheet, bool shouldReverse)
    {
        m_states[name] = std::make_pair(spriteSheet, shouldReverse);
        if (m_currentStateName.empty())
            m_currentStateName = name;
    }

    void AnimationStates::removeState(const std::string &name)
    {
        m_states.erase(name);
    }

    void AnimationStates::setState(const std::string &name, bool skip)
    {
        if (skip) {
            m_currentStateName = name;
            m_states.at(name).first.reset();
            return;
        }
        if (m_currentStateName == name || m_switchingState)
            return;
        m_switchingState = true;
        m_reversing = false;
        m_nextStateName = name;
        m_states.at(m_nextStateName).first.reset();
    }

    void AnimationStates::update(float dt)
    {
        if (m_currentStateName.empty())
            return;
        m_states.at(m_currentStateName).first.update(dt);
        if (m_switchingState && (m_states.at(m_currentStateName).first.isFinished() || m_states.at(m_currentStateName).first.isLooping())) {

            if (m_states.at(m_currentStateName).second && !m_reversing) {
                m_states.at(m_currentStateName).first.reverse();
                m_reversing = true;
                return;
            }

            m_currentStateName = m_nextStateName;
            m_states.at(m_nextStateName).first.reset();
            m_switchingState = false;
        }
    }

    const SpriteSheet &AnimationStates::getCurrentState() const
    {
        return m_states.at(m_currentStateName).first;
    }

    std::string AnimationStates::getCurrentStateName() const
    {
        return m_currentStateName;
    }

}