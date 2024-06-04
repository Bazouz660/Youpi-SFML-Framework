#ifndef DRAWABLE_HPP_
#define DRAWABLE_HPP_

#include "YPI/math.hpp"
#include "YPI/graphics.hpp"
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