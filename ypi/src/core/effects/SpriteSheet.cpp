#include "SpriteSheet.hpp"

namespace exng::effect {

    void SpriteSheet::update(float dt)
    {
        if (m_frames.size() == 0)
            return;
        m_timer += dt;
        if (m_timer >= m_frames.at(m_frameIndex).frameDuration) {
            m_timer = 0;
            m_frameIndex++;
            if (m_frameIndex >= m_frames.size()) {
                if (m_loop) {
                    m_frameIndex = 0;
                } else {
                    m_frameIndex = m_frames.size() - 1;
                    m_finished = true;
                }
            }
        }
    }

    void SpriteSheet::reset()
    {
        m_timer = 0;
        m_frameIndex = 0;
        m_finished = false;
        if (m_reversed) {
            reverse();
            m_reversed = false;
        }
    }

    void SpriteSheet::addFrame(IntRect frame, float frameDuration)
    {
        m_frames.push_back({frame, frameDuration});
        if (m_frames.size() == 1)
            m_frameIndex = 0;
    }

    IntRect SpriteSheet::getFrame() const
    {
        return m_frames.at(m_frameIndex).frame;
    }

    void SpriteSheet::reverse()
    {
        std::reverse(m_frames.begin(), m_frames.end());
        m_frameIndex = m_frames.size() - 1 - m_frameIndex;
        if (m_finished)
            m_finished = false;
        m_reversed = !m_reversed;
    }

    bool SpriteSheet::isFinished() const {
        bool finished = m_finished;
        if (m_loop)
            finished = false;
        if (m_frames.size() == 0)
            finished = true;
        return finished;
    }

}
