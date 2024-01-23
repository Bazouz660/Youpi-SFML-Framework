#ifndef DRAWABLE_HPP_
#define DRAWABLE_HPP_

#include "ypi/src/core/vector2/Vector2.hpp"
#include "ypi/lib_headers/sfml.hpp"
#include <vector>

namespace comp {

    struct VertexArrayDrawable
    {
        sf::VertexArray vertices;
        sf::Color color = sf::Color::White;
        sf::PrimitiveType primitiveType = sf::PrimitiveType::Triangles;
    };

    struct CircleDrawable
    {
        sf::CircleShape shape;
    };

}

#endif // !DRAWABLE_HPP_