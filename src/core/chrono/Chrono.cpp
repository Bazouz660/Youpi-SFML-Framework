#include "Chrono.hpp"

#include "logger.hpp"

namespace ypi {


    sf::Time Chrono::getElapsedTime()
    {
        return m_clock.getElapsedTime();
    }

    void Chrono::update()
    {
        if (!m_paused) {
            auto& clock = m_clock;
            auto& elapsedTime = m_elapsedTime;
            auto& prevElapsedTime = m_prevElapsedTime;
            auto& frameDt = m_frameDt;

            elapsedTime = clock.getElapsedTime();
            frameDt = elapsedTime - prevElapsedTime;
            prevElapsedTime = elapsedTime;
        } else {
            m_prevElapsedTime = m_clock.getElapsedTime();
        }
    }

    sf::Time Chrono::getFrameDt()
    {
        return m_frameDt;
    }

    float Chrono::getFrameRate()
    {
        return 1.0f / m_frameDt.asSeconds();
    }

    void Chrono::pause()
    {
        if (!m_paused)
        {
            m_paused = true;
            m_pauseTime = m_elapsedTime;
        }
    }

    void Chrono::resume()
    {
        if (m_paused)
        {
            m_paused = false;
            m_prevElapsedTime = m_clock.getElapsedTime();
            m_clock.restart();
            m_elapsedTime = m_pauseTime;
        }
    }

    void Chrono::reset()
    {
        m_clock.restart();
        m_elapsedTime = sf::Time::Zero;
        m_prevElapsedTime = sf::Time::Zero;
        m_frameDt = sf::Time::Zero;
    }
}