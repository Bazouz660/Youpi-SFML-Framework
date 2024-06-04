#include "Timer.hpp"

namespace ypi
{
    void Timer::update(float dt)
    {
        if (!m_running || m_timeout < 0) return;

        timeBuffer += dt;
        if (timeBuffer >= m_timeout)
        {
            m_callback();
            if (!m_repeat)
                m_running = false;
            timeBuffer = 0;
        }
    }

    void AsyncTimer::start()
    {
        m_running = true;
        m_future = std::async(std::launch::async, [this]() {
            while (m_running) {
                std::this_thread::sleep_for(m_timeout);
                if (m_running) {  // Check again after sleeping
                    m_callback();
                    if (!m_repeat) {
                        m_running = false;
                    }
                }
            }
        });
    }

    void AsyncTimer::stop()
    {
        m_running = false;
        if (m_future.valid()) {
            m_future.get();  // Wait for the async task to finish
        }
    }

} // namespace ypi