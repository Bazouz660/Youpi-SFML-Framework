#ifndef YPI_SCENEMANAGER_HPP
#define YPI_SCENEMANAGER_HPP

#include "AScene.hpp"

#include <map>
#include <memory>
#include <string>
#include <mutex>
#include <atomic>

namespace ypi
{
    class SceneManager
    {
    public:
        SceneManager();

        void addScene(const std::string& name, const_sptr_ref<AScene> scene);
        void removeScene(const std::string& name);
        void loadScene(const std::string& name);
        void unloadCurrentScene();

        void reloadCurrentScene();

        void update(float dt);
        void handleEvent(const sf::Event& event, const Vector2f& mousePos);
        void draw(ypi::RenderTexture& target );

        const_sptr_ref<AScene> getCurrentScene();
    private:
        std::map<std::string, std::shared_ptr<AScene>> m_scenes;
        std::shared_ptr<AScene> m_currentScene;
        std::shared_ptr<AScene> m_nextScene;
        std::atomic<bool> m_reloadRequested;
    };
}

#endif // !YPI_SCENEMANAGER_HPP