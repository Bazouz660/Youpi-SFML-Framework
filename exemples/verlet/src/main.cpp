#include "YPI/ecs.hpp"
#include "YPI/data.hpp"
#include "YPI/math.hpp"
#include "YPI/graphics.hpp"

#include "entityConstructors.hpp"
#include "components/components.hpp"
#include "systems/systems.hpp"

#include "gridVal.h"

int main()
{
    ypi::RenderWindow window;
    ypi::ecs::registry registry;
    ypi::Chrono chrono;
    sf::Event event;
    ypi::gui::Container guiContainer;

    ypi::Grid<GridVal, GetBoxFunc, Equal, PairHash> grid(800, 600, 15, [&](const GridVal& entity) {
        ypi::Rect<float> aabb;
        auto verlet = entity.second;
        aabb.left = verlet->position.x - verlet->radius;
        aabb.top = verlet->position.y - verlet->radius;
        aabb.width = verlet->radius * 2;
        aabb.height = verlet->radius * 2;
        return aabb;
    });

    float spawnTimer = 0.0f;
    float fpsRefreshTimer = 0.0f;
    float slowMotionFactor = 1.0f;

    ypi::ResourceManager::loadFromFolder(ypi::ResourceType::SoundBuffer, "asset/audio/sound", true);
    ypi::ResourceManager::loadFromFolder(ypi::ResourceType::Texture, "asset/texture", true);
    ypi::ResourceManager::loadFromFolder(ypi::ResourceType::Font, "asset/font", true);

    ypi::ResourceManager::waitForLoading();

    auto verletSystem = sys::VerletSystem();
    auto renderCircle = sys::RenderCircle();
    auto dragSystem = sys::DragSystem();

    auto fpsLabel = std::make_shared<ypi::gui::Label>();
    fpsLabel->setFont(ypi::ResourceManager::getFont("font", "Retrocompute"));
    fpsLabel->setCharacterSize(15);
    fpsLabel->setString("fps: 0");

    auto nbBallsLabel = std::make_shared<ypi::gui::Label>();
    nbBallsLabel->setFont(ypi::ResourceManager::getFont("font", "Retrocompute"));
    nbBallsLabel->setCharacterSize(15);
    nbBallsLabel->setString("nb balls: 0");
    nbBallsLabel->setPosition({0, 15});

    guiContainer.addComponent("fpsLabel", fpsLabel);
    guiContainer.addComponent("nbBallsLabel", nbBallsLabel);

    window.create(sf::VideoMode(800, 600), "Verlet", sf::Style::Close | sf::Style::Titlebar);
    window.setVerticalSyncEnabled(true);

    constructors::createVerletBall(registry, {200.0f, 200.0f}, 50.0f, ypi::coloring::rainbow(chrono.getElapsedTime().asSeconds()));

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
            guiContainer.handleAnyEvents(event, ypi::context::getMousePosition().to<float>());
        }

        if (!window.isOpen())
            break;

        spawnTimer += dt;
        if (spawnTimer >= 0.05f && chrono.getFrameRate() > 60.0f) {
            sf::Color color = ypi::coloring::rainbow(chrono.getElapsedTime().asSeconds());

            int nbBalls = 4;
            float posX = 400;
            float posY = 200;
            for (int i(0); i < nbBalls; ++i) {
                float radius = ypi::nbgen::between(2.0f, 4.0f);
                constructors::createVerletBall(registry, {posX, posY + (i * 20)}, radius, color);
            }

            auto view = registry.view<comp::Verlet>();
            nbBallsLabel->setString("nb balls: " + std::to_string(view.size()));
            spawnTimer = 0.0f;
        }

        //dragSystem->update(window);
        verletSystem.update(registry, grid, slowMotionFactor);
        dragSystem.update(registry, grid, window);

        fpsRefreshTimer += dt;
        if (fpsRefreshTimer >= 0.2f) {
            fpsRefreshTimer = 0.0f;
            fpsLabel->setString("fps: " + std::to_string(int(1.0f / dt)));
        }

        window.clear();

        renderCircle.render(registry, window);
        guiContainer.render(window);

        window.display();
    }

    ypi::ResourceManager::stop();
}