#include "ypi/src/core/window/Window.hpp"
#include "ypi/src/core/ecs/coordinator.hpp"
#include "ypi/src/core/chrono/Chrono.hpp"
#include "ypi/src/core/resource_manager/ResourceManager.hpp"
#include "ypi/src/core/gui/Label.hpp"
#include "ypi/src/core/gui/Container.hpp"
#include "ypi/src/helper/color.hpp"
#include "entityConstructors.hpp"
#include "components/components.hpp"
#include "systems/systems.hpp"

int main()
{
    exng::Window window;
    exng::Coordinator coordinator;
    exng::Chrono chrono;
    sf::Event event;
    exng::gui::Container guiContainer;
    float spawnTimer = 0.0f;
    float fpsRefreshTimer = 0.0f;
    float slowMotionFactor = 1.0f;

    exng::ResourceManager::loadFromFolder(exng::ResourceType::SoundBuffer, "asset/sound", true);
    exng::ResourceManager::loadFromFolder(exng::ResourceType::Texture, "asset/texture", true);
    exng::ResourceManager::loadFromFolder(exng::ResourceType::Font, "asset/font", true);

    exng::ResourceManager::waitForLoading();

    coordinator.init();

    coordinator.registerComponent<comp::Verlet>();
    coordinator.registerComponent<comp::Sound>();
    coordinator.registerComponent<comp::Transform>();
    coordinator.registerComponent<comp::CircleDrawable>();
    coordinator.registerComponent<comp::VertexArrayDrawable>();

    auto verletSystem = coordinator.createSystem<sys::VerletSystem,
        comp::Verlet,
        comp::Transform
    >();

    auto dragSystem = coordinator.createSystem<sys::DragSystem,
        comp::Verlet,
        comp::CircleDrawable
    >();

    auto renderCircle = coordinator.createSystem<sys::RenderCircle,
        comp::CircleDrawable,
        comp::Transform
    >();

    auto fpsLabel = std::make_shared<exng::gui::Label>(exng::Vector2f(0, 0), "fps: 0");
    fpsLabel->getText().setFont(exng::ResourceManager::getFont("font", "Retrocompute"));
    fpsLabel->getText().setCharacterSize(15);

    auto nbBallsLabel = std::make_shared<exng::gui::Label>(exng::Vector2f(0, 15), "nb balls: 0");
    nbBallsLabel->getText().setFont(exng::ResourceManager::getFont("font", "Retrocompute"));
    nbBallsLabel->getText().setCharacterSize(15);

    guiContainer.addComponent(fpsLabel);
    guiContainer.addComponent(nbBallsLabel);

    window.create("Test", {800, 600});
    window.setVerticalSyncEnabled(true);

    //constructors::createVerletBall(coordinator, {200.0f, 200.0f}, 20.0f, exng::coloring::rainbow(chrono.getElapsedTime().asSeconds()));

    while (window.isOpen()) {

        chrono.update();
        float dt = chrono.getFrameDt().asSeconds();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                    break;
                }
                if (event.key.code == sf::Keyboard::Space) {
                    slowMotionFactor = slowMotionFactor == 1.0f ? 0.1f : 1.0f;
                }
            }
            guiContainer.handleEvent(event);
        }

        if (!window.isOpen())
            break;

        spawnTimer += dt;
        if (spawnTimer >= 0.05f && chrono.getFrameRate() > 60.0f) {
            sf::Color color = exng::coloring::rainbow(chrono.getElapsedTime().asSeconds());

            int nbBalls = 4;
            float posX = 400;
            float posY = 200;
            for (int i(0); i < nbBalls; ++i) {
                float radius = exng::nbgen::between(2.0f, 8.0f);
                constructors::createVerletBall(coordinator, {posX, posY + (i * 20)}, radius, color);
            }

            nbBallsLabel->getText().setString("nb balls: " + std::to_string(coordinator.getEntities().size()));
            spawnTimer = 0.0f;
        }

        dragSystem->update(window);
        verletSystem->update(slowMotionFactor);
        guiContainer.update();

        fpsRefreshTimer += dt;
        if (fpsRefreshTimer >= 0.2f) {
            fpsRefreshTimer = 0.0f;
            fpsLabel->getText().setString("fps: " + std::to_string(int(1.0f / dt)));
        }

        window.clear();

        renderCircle->render(window);
        guiContainer.render(window);

        window.display();
    }

    exng::ResourceManager::stop();
}