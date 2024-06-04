#ifndef YPI_EFFECTS_SPRITESHEET_HPP
#define YPI_EFFECTS_SPRITESHEET_HPP

#include "../../../lib_headers/common.hpp"
#include "../../../lib_headers/sfml.hpp"
#include "../../math/Rect.hpp"
#include "../../math/Vector2.hpp"

namespace ypi::effect {

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

#endif // !YPI_EFFECTS_SPRITESHEET_HPP