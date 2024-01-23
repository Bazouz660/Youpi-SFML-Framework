#include "ypi/src/core/window/Window.hpp"
#include "ypi/src/core/ecs/coordinator.hpp"
#include "ypi/src/core/chrono/Chrono.hpp"
#include "ypi/src/core/resource_manager/ResourceManager.hpp"
#include "entityConstructors.hpp"
#include "components/components.hpp"
#include "systems/systems.hpp"

int main()
{
    exng::Window window;
    exng::Coordinator coordinator;
    exng::Chrono chrono;
    sf::Event event;
    float spawnTimer = 0.0f;

    exng::ResourceManager::loadFromFolder(exng::ResourceType::SoundBuffer, "asset/sound", true);

    coordinator.init();

    coordinator.registerComponent<comp::Transform>();
    coordinator.registerComponent<comp::Verlet>();
    coordinator.registerComponent<comp::VertexArrayDrawable>();
    coordinator.registerComponent<comp::Sound>();

    auto vertexRenderSystem = coordinator.createSystem<sys::RenderVertexArray,
        comp::Transform,
        comp::VertexArrayDrawable
    >();

    auto verletSystem = coordinator.createSystem<sys::VerletSystem,
        comp::Transform,
        comp::Verlet
    >();

    window.create("Test", {800, 600});

    while (window.isOpen()) {

        chrono.update();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
        }

        if (!window.isOpen())
            break;

        spawnTimer += chrono.getFrameDt().asSeconds();
        if (spawnTimer >= 0.1f) {
            spawnTimer = 0.0f;
            constructors::createVerletBall(coordinator, {400.0f, 300.0f}, 5.0f, sf::Color::White);
        }

        verletSystem->update(chrono.getFrameDt().asSeconds());

        window.clear();

        vertexRenderSystem->render(window);

        window.display();
    }

    exng::ResourceManager::stop();
}