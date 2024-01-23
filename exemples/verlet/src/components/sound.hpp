#pragma once

#include "ypi/src/core/vector2/Vector2.hpp"
#include "ypi/lib_headers/sfml.hpp"
#include <memory>

namespace comp {

    struct Sound
    {
        std::shared_ptr<sf::Sound> sound;
    };

}