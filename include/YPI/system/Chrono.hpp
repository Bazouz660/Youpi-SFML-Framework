#pragma once

#include "../../lib_headers/common.hpp"

namespace ypi {

    class Chrono
    {
    public:
        Chrono() = default;

        sf::Time getElapsedTime();

        void update();

        sf::Time getFrameDt();

        float getFrameRate();

        void pause();
        void resume();
        void reset();

    private:
        sf::Time m_frameDt;
        sf::Time m_elapsedTime;
        sf::Time m_prevElapsedTime;
        sf::Clock m_clock;

        bool m_paused = false;
        sf::Time m_pauseTime;
    };
}