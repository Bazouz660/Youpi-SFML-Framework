#ifndef ENTITYCONSTRUCTORS_HPP_
#define ENTITYCONSTRUCTORS_HPP_

#include "YPI/ecs.hpp"
#include "YPI/data.hpp"
#include "YPI/math.hpp"
#include "YPI/graphics.hpp"

#include "components/components.hpp"

namespace constructors {

    entt::entity createVerletBall(entt::registry &registry, ypi::Vector2f position, float radius, sf::Color color)
    {
        entt::entity entity = registry.create();

        comp::Verlet verlet;
        verlet.radius = radius;
        verlet.position = position;
        verlet.oldPosition = position;
        verlet.oldPosition -= {1.f, 0.0f};
        verlet.acceleration = {0.0f, 0.0f};
        verlet.mass = radius * radius;

        comp::CircleDrawable circle;
        circle.shape.setRadius(radius);
        circle.shape.setFillColor(color);

        comp::Transform transform;
        transform.setPosition(position);
        transform.setOrigin({radius, radius});

        registry.emplace<comp::Verlet>(entity, verlet);
        registry.emplace<comp::CircleDrawable>(entity, circle);
        registry.emplace<comp::Transform>(entity, transform);

        return entity;
    }

}

#endif // !ENTITYCONSTRUCTORS_HPP_