#ifndef ENTITYCONSTRUCTORS_HPP_
#define ENTITYCONSTRUCTORS_HPP_

#include "ypi/src/core/ecs/coordinator.hpp"
#include "ypi/src/helper/math/math.hpp"
#include "ypi/src/core/resource_manager/ResourceManager.hpp"
#include "ypi/src/helper/number_generator/numberGenerator.hpp"
#include "components/components.hpp"

namespace constructors {

    exng::Entity createVerletBall(exng::Coordinator &coordinator, exng::Vector2f position, float radius, sf::Color color) {
        exng::Entity entity = coordinator.createEntity();

        comp::Transform transform;
        transform.setPosition(position);

        comp::Verlet verlet;
        verlet.radius = radius;
        verlet.position = position;
        verlet.oldPosition = position;
        verlet.acceleration = {1000000.f, 0.0f};

        comp::VertexArrayDrawable drawable;
        drawable.primitiveType = sf::PrimitiveType::TriangleFan;

        //comp::Sound sound;
        //sound.sound = std::make_shared<sf::Sound>();
        //sound.sound->setBuffer(exng::ResourceManager::getSoundBuffer("sound", "bounce"));

        // generate a circle with x segments
        int segments = 8 + (radius * 2);
        float angle = 0.0f;
        float angleStep = 360.0f / segments;
        for (int i = 0; i < segments; i++) {
            drawable.vertices.append(sf::Vertex(sf::Vector2f(radius * cos(angle * PI / 180.0f), radius * sin(angle * PI / 180.0f)), color));
            angle += angleStep;
        }

        coordinator.addComponent(entity, transform);
        coordinator.addComponent(entity, verlet);
        coordinator.addComponent(entity, drawable);
        //coordinator.addComponent(entity, sound);

        return entity;
    }

}

#endif // !ENTITYCONSTRUCTORS_HPP_