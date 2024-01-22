#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include "AScene.hpp"
#include "Exception.hpp"

namespace exng {

    class SceneManager
    {
    public:
        SceneManager() {}
        ~SceneManager() {}

        void registerScene(const std::string& name) {
            if (std::find(m_registeredScenes.begin(), m_registeredScenes.end(), name) != m_registeredScenes.end()) {
                THROW("Scene already registered");
            }

            m_registeredScenes.push_back(name);
        }

        void unregisterScene(const std::string& name) {
            if (std::find(m_registeredScenes.begin(), m_registeredScenes.end(), name) == m_registeredScenes.end()) {
                THROW("Scene not registered");
            }

            if (m_loadedScenes.find(name) != m_loadedScenes.end()) {
                THROW("Scene still loaded");
            }

            m_registeredScenes.erase(std::remove(m_registeredScenes.begin(), m_registeredScenes.end(), name), m_registeredScenes.end());
        }

        bool isSceneRegistered(const std::string& name) {
            return std::find(m_registeredScenes.begin(), m_registeredScenes.end(), name) != m_registeredScenes.end();
        }

        void loadScene(const std::string& name, std::shared_ptr<AScene> scene) {
            if (m_loadedScenes.find(name) != m_loadedScenes.end()) {
                THROW("Scene already loaded");
            }

            if (std::find(m_registeredScenes.begin(), m_registeredScenes.end(), name) == m_registeredScenes.end()) {
                THROW("Scene not registered");
            }

            m_loadedScenes[name] = scene;
        }

        void unloadScene(const std::string& name) {
            if (m_loadedScenes.find(name) == m_loadedScenes.end()) {
                THROW("Scene not loaded");
            }

            m_loadedScenes.erase(name);
        }

        bool isSceneLoaded(const std::string& name) {
            return m_loadedScenes.find(name) != m_loadedScenes.end();
        }

        void switchScene(const std::string& name) {
            if (m_loadedScenes.find(name) == m_loadedScenes.end()) {
                THROW("Scene not loaded");
            }

            if (m_currentScene) {
                m_currentScene->onExit();
            }
            m_currentScene = m_loadedScenes[name];
            m_currentScene->onEnter();
        }

        void update(float dt) {
            if (m_currentScene) {
                m_currentScene->update(dt);
            }
        }

        void render(Window& target) {
            if (m_currentScene) {
                m_currentScene->render(target);
            }
        }

        void processEvents(sf::Event event) {
            if (m_currentScene) {
                m_currentScene->processEvents(event);
            }
        }

        std::shared_ptr<AScene> getCurrentScene() {
            return m_currentScene;
        }

    private:
        std::shared_ptr<AScene> m_currentScene;
        std::unordered_map<std::string, std::shared_ptr<AScene>> m_loadedScenes;
        std::vector<std::string> m_registeredScenes;
    };
}