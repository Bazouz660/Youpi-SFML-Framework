#ifndef ENTITYCONSTRUCTORS_HPP_
#define ENTITYCONSTRUCTORS_HPP_

#include "ypi/src/core/ecs/coordinator.hpp"
#include "ypi/src/helper/math/math.hpp"
#include "ypi/src/core/resource_manager/ResourceManager.hpp"
#include "ypi/src/helper/number_generator/numberGenerator.hpp"
#include "components/components.hpp"

#include "ypi/lib_headers/entt.hpp"

namespace constructors {

    entt::entity createVerletBall(entt::registry &registry, exng::Vector2f position, float radius, sf::Color color)
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