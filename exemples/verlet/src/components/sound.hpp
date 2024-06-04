#pragma once

#include "YPI/math.hpp"
#include "YPI/graphics.hpp"
#include <memory>

namespace comp {

    struct Sound
    {
        std::shared_ptr<sf::Sound> sound;
    };

}