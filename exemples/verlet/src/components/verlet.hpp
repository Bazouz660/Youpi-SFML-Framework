#pragma once

#include "YPI/math.hpp"

namespace comp {

    struct Verlet
    {
        float radius = 5.0f;
        float mass = 1.0f;
        ypi::Vector2f position;
        ypi::Vector2f oldPosition;
        ypi::Vector2f acceleration;
    };

}