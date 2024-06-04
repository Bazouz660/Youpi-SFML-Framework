#include "SceneManager.hpp"

namespace ypi
{
    SceneManager::SceneManager()
        : m_currentScene(nullptr)
    {}

    void SceneManager::addScene(const std::string& name, const_sptr_ref<AScene> scene)
    {
        m_scenes[name] = scene;
    }

    void SceneManager::removeScene(const std::string& name)
    {
        m_scenes.erase(name);
    }

    void SceneManager::loadScene(const std::string& name)
    {
        auto it = m_scenes.find(name);
        if (it != m_scenes.end())
            m_nextScene = it->second;
    }

    void SceneManager::unloadCurrentScene()
    {
        if (m_currentScene)
        {
            m_currentScene->unload();
            m_currentScene = nullptr;
        }
    }

    void SceneManager::update(float dt)
    {
        if (m_reloadRequested)
        {
            if (m_currentScene)
            {
                m_currentScene->unload();
                m_currentScene->load();
            }
            m_reloadRequested = false;
        }

        if (m_nextScene)
        {
            if (m_currentScene)
                m_currentScene->unload();
            m_currentScene = m_nextScene;
            m_nextScene = nullptr;
            m_currentScene->load();
        }

        if (m_currentScene) {
            m_currentScene->update(dt);
        }
    }

    void SceneManager::handleEvent(const sf::Event& event, const Vector2f& mousePos)
    {
        if (m_currentScene)
        {
            m_currentScene->handleEvent(event, mousePos);
        }
    }

    void SceneManager::draw(ypi::RenderTexture& target)
    {
        if (m_currentScene)
        {
            m_currentScene->draw(target);
        }
    }

    const_sptr_ref<AScene> SceneManager::getCurrentScene()
    {
        return m_currentScene;
    }

    void SceneManager::reloadCurrentScene()
    {
        m_reloadRequested = true;
    }
}