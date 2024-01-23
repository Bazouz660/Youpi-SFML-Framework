#pragma once

#include "ypi/src/core/vector2/Vector2.hpp"

namespace comp {

    struct Verlet
    {
        float radius = 5.0f;
        exng::Vector2f position;
        exng::Vector2f oldPosition;
        exng::Vector2f acceleration;
    };

}