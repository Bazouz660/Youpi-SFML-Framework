#ifndef ENTITYCONSTRUCTORS_HPP_
#define ENTITYCONSTRUCTORS_HPP_

#include "ypi/src/core/ecs/coordinator.hpp"
#include "ypi/src/helper/math/math.hpp"
#include "ypi/src/core/resource_manager/ResourceManager.hpp"
#include "ypi/src/helper/number_generator/numberGenerator.hpp"
#include "components/components.hpp"

namespace constructors {

    exng::Entity createVerletBall(exng::Coordinator &coordinator, exng::Vector2f position, float radius, sf::Color color)
    {
        exng::Entity entity = coordinator.createEntity();

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

        coordinator.addComponent(entity, verlet);
        coordinator.addComponent(entity, circle);
        coordinator.addComponent(entity, transform);

        return entity;
    }

}

#endif // !ENTITYCONSTRUCTORS_HPP_