#ifndef EXNG_EFFECTS_SPRITESHEET_HPP
#define EXNG_EFFECTS_SPRITESHEET_HPP

#include "ypi/lib_headers/common.hpp"
#include "ypi/lib_headers/sfml.hpp"
#include "ypi/src/core/rect/Rect.hpp"
#include "ypi/src/core/vector2/Vector2.hpp"

namespace exng::effect {

    class SpriteSheet {
        public:
            SpriteSheet() = default;
            ~SpriteSheet() = default;

            SpriteSheet(const SpriteSheet &other) = default;

            void update(float dt);
            void reset();
            void addFrame(IntRect frame, float frameDuration);
            void reverse();
            void setLoop(bool loop) { m_loop = loop; };
            bool isFinished() const;
            bool isLooping() const { return m_loop; };
            bool isReversed() const { return m_reversed; };

            IntRect getFrame() const;

        private:
            struct frame
            {
                IntRect frame;
                float frameDuration;
            };

            std::vector<frame> m_frames;
            int m_frameIndex = 0;
            float m_timer = 0;
            bool m_loop = true;
            bool m_finished = false;
            bool m_reversed = false;
    };

}

#endif // !EXNG_EFFECTS_SPRITESHEET_HPP