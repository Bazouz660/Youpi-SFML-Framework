#ifndef YPI_TIMER_HPP
#define YPI_TIMER_HPP

#include <functional>
#include <future>
#include <chrono>

namespace ypi
{
    /**
     * @brief Timer class
     *
     * This class provides a simple timer functionality.
     * It can be used to call a function after a certain amount of time has passed.
     * It is useful for creating time-based events.
     * The update function should be called every frame to update the timer.
     */
    class Timer
    {
    public:
        Timer() = default;
        ~Timer() = default;

        void update(float dt);
        void setTimeout(float time) { m_timeout = time; }
        void setCallback(std::function<void()> callback) { m_callback = callback; }
        void setRepeat(bool repeat) { m_repeat = repeat; }
        void start() { m_running = true; }
        void stop() { m_running = false; }
        bool isRunning() { return m_running; }

    private:
        float timeBuffer = 0;
        float m_timeout = -1;
        bool m_repeat = true;
        bool m_running = true;
        std::function<void()> m_callback = [](){};
    };



    class AsyncTimer
    {
    public:
        AsyncTimer() : m_running(false) {}
        ~AsyncTimer() { stop(); }

        void setTimeout(float time) { m_timeout = std::chrono::milliseconds(static_cast<int>(time * 1000)); }
        void setCallback(std::function<void()> callback) { m_callback = callback; }
        void setRepeat(bool repeat) { m_repeat = repeat; }

        void start();
        void stop();
        bool isRunning() { return m_running; }

    private:
        std::chrono::milliseconds m_timeout;
        bool m_repeat = false;
        bool m_running = false;
        std::function<void()> m_callback = [](){};
        std::future<void> m_future;
    };
}

#endif // !YPI_TIMER_HPP