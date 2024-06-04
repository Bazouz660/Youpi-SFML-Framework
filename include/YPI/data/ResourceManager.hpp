/*
 *  Author: Basile Trebus--Hamann
 *  Create Time: 2023-07-06 22:23:37
 * @ Modified by: Basile Trebus--Hamann
 * @ Modified time: 2023-07-15 03:01:33
 *  Description:
 */

#pragma once

#include <filesystem>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <atomic>
#include <algorithm>

#include "../../lib_headers/common.hpp"
#include "../system/parsing.hpp"
#include "../system/logger.hpp"
#include "../system/Observer.hpp"
#include "../system/Exception.hpp"
#include "../data/MusicManager.hpp"
#include "../data/SoundManager.hpp"

namespace ypi {

    enum class ResourceType {
        Texture,
        Font,
        SoundBuffer,
        Image,
        Music
    };

    class ResourceManager {
        public:
            static bool isLoading() { return getInstance().m_isLoading; }

            static sf::Image& getTextureImage(const std::string& location, const std::string &name)
            {
                ResourceManager& instance = getInstance();
                auto& m_images = instance.m_images;
                std::string noExtensionName = parsing::removeExtension(name);

                auto& texture = getTexture(location, name);
                std::shared_ptr<sf::Image> image = std::make_shared<sf::Image>(instance.m_textures.at(location)->at(noExtensionName)->copyToImage());
                if (m_images.find(location) == m_images.end())
                    m_images[location] = std::make_shared<ResourceMap<sf::Image>>();
                m_images[location]->insert({noExtensionName, image});
                return *m_images.at(location)->at(noExtensionName);
            }

            static sf::Texture& getTexture(const std::string& location, const std::string &name)
            {
                try {
                    return getResourceInternal<sf::Texture>(location, name);
                } catch (const Exception& e) {
                    ypi::logger::error(e.what());
                    return getResourceInternal<sf::Texture>("placeholder", "texture");
                }
            }

            static sf::Font& getFont(const std::string& location, const std::string& fontName)
            {
                return getResourceInternal<sf::Font>(location, fontName);
            }

            static sf::SoundBuffer& getSoundBuffer(const std::string& location, const std::string& name)
            {
                return getResourceInternal<sf::SoundBuffer>(location, name);
            }

            static void loadResource(const ResourceType& type, const std::string& filePath)
            {
                switch (type) {
                    case ResourceType::Texture:
                        loadResource<sf::Texture>(filePath);
                        break;
                    case ResourceType::Font:
                        loadResource<sf::Font>(filePath);
                        break;
                    case ResourceType::SoundBuffer:
                        loadResource<sf::SoundBuffer>(filePath);
                        break;
                    case ResourceType::Image:
                        loadResource<sf::Image>(filePath);
                        break;
                    case ResourceType::Music:
                        loadResource<sf::Music>(filePath);
                        break;
                    default:
                        THROW("Unknown resource type");
                }
            }

            static void waitForLoading()
            {
                while (isLoading()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }

            static void loadFromFolder(const ResourceType& type, const std::string& folderPath, bool recursive = false)
            {
                std::filesystem::path path(folderPath);
                std::string directoryName = path.filename().string();

                // check if the folder exists
                if (!std::filesystem::exists(folderPath)) {
                    logger::error("Resource folder not found: " + folderPath);
                    return;
                }

                // iterate over the directory
                for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
                    // if the entry is a directory and recursive is true, load textures from it

                    if (std::filesystem::is_directory(entry) && recursive) {
                        loadFromFolder(type, entry.path().string(), recursive);
                    } else {
                        // if the entry is a file, load it as a texture
                        loadResource(type, entry.path().string());
                    }
                }
            }

            static void stop()
            {
                getInstance().m_shouldClose = true;
                getInstance().m_condVar.notify_all();
            }

            static void addProgressObserver(const_sptr_ref<AObserver<float>> observer)
            {
                getInstance().m_progressSubject.addObserver(observer);
            }

            static void addDoneObserver(const_sptr_ref<AObserver<bool>> observer)
            {
                getInstance().m_doneSubject.addObserver(observer);
            }

        private:
            ResourceManager()
            {
                createPlaceholderTexture();
                m_isLoading = false;
                m_threadCount = std::thread::hardware_concurrency(); // get the number of threads supported by the hardware
                m_loadingThreads.resize(m_threadCount);
                for (auto& thread : m_loadingThreads) {
                    thread = std::thread([this]() {
                        std::unique_lock<std::mutex> lock(m_mutex);
                        while (true) {
                            m_condVar.wait(lock, [this]() { return !m_loadingTasks.empty() || m_shouldClose; });
                            if (m_shouldClose && m_loadingTasks.empty()) {
                                return;
                            }
                            m_loadingTasks.front()();
                            m_loadingTasks.pop();
                            if (m_loadingTasks.empty()) {
                                m_isLoading = false;
                                waitForLoading();
                                m_doneSubject.notify(true);
                            }
                        }
                    });
                }
            }

            ~ResourceManager()
            {
                for (auto& thread : m_loadingThreads) {
                    if (thread.joinable()) {
                        thread.join();
                    }
                }
            }

            ResourceManager(const ResourceManager&) = delete;
            ResourceManager& operator=(const ResourceManager&) = delete;

            template<typename T>
            using ResourceMap = std::map<std::string, std::shared_ptr<T>>;
            template<typename T>
            using ResourceMultimap = std::map<std::string, std::shared_ptr<ResourceMap<T>>>;

            std::atomic<bool> m_shouldClose = false;

            template<typename T>
            static void loadResource(const std::string& filePath)
            {
                ResourceManager& instance = getInstance();
                instance.m_isLoading = true;
                instance.m_totalTasks++; // Increase total tasks

                instance.m_loadingTasks.push([filePath]() {
                    auto& map = getMap<T>();
                    std::filesystem::path path(filePath);
                    std::string noExtensionName = parsing::removeExtension(path.filename().string());
                    std::string directory = path.parent_path().string();
                    std::string directoryName = path.parent_path().filename().string();

                    int status = 0;
                    std::shared_ptr<T> resource = std::make_shared<T>();
                    if constexpr (std::is_same_v<T, sf::Music>) {
                        // check if the music is already loaded
                        if (MusicManager::isLoaded(noExtensionName)) {
                            updateCompletedTasks();
                            return false;
                        }
                        status = resource->openFromFile(filePath);
                        resource->setVolume(MusicManager::getMusicVolume());
                        MusicManager::addTrack(noExtensionName, resource.get());
                    } else if constexpr (std::is_same_v<T, sf::SoundBuffer>) {
                        // check if the sound buffer is already loaded
                        if (SoundManager::isLoaded(noExtensionName)) {
                            updateCompletedTasks();
                            return false;
                        }
                        status = resource->loadFromFile(filePath);
                        SoundManager::addSoundBuffer(noExtensionName, resource);
                    } else {
                        status = resource->loadFromFile(filePath);
                        if (status == 0) {
                            updateCompletedTasks();
                            return false;
                        }
                    }

                    if (map.find(directoryName) == map.end())
                        map[directoryName] = std::make_shared<ResourceMap<T>>();
                    if (map.at(directoryName)->find(noExtensionName) == map.at(directoryName)->end()) {
                        map[directoryName]->insert({noExtensionName, resource});
                    } else {
                        updateCompletedTasks();
                        return false;
                    }

                    logger::success() << "Loaded resource: " << "\"" << filePath << "\"";
                    updateCompletedTasks();
                    return true;
                });

                instance.m_condVar.notify_one();
            }

            void createPlaceholderTexture()
            {
                // Generate placeholder texture for missing textures
                unsigned int size = 16;

                sf::Texture placeholderTexture;
                placeholderTexture.create(size, size);
                sf::Image placeholderImage;
                placeholderImage.create(size, size, sf::Color::Magenta);
                // create checkerboard pattern
                for (unsigned int i = 0; i < size; i++) {
                    for (unsigned int j = 0; j < size; j++) {
                        if ((i + j) % 2 == 0)
                            placeholderImage.setPixel(i, j, sf::Color(25, 25, 25, 255));
                    }
                }
                placeholderTexture.update(placeholderImage);
                placeholderTexture.setRepeated(true);

                // load placeholder texture
                m_textures["placeholder"] = std::make_shared<ResourceMap<sf::Texture>>();

                // save placeholder texture in the map
                m_textures["placeholder"]->insert({"texture", std::make_shared<sf::Texture>(placeholderTexture)});
            }

            // generic get resource function
            template<typename T>
            static T& getResourceInternal(const std::string& location, const std::string& name)
            {
                auto& map = getMap<T>();

                // remove extension from name
                std::string noExtensionName = parsing::removeExtension(name);

                assertResourceExist<T>(location, noExtensionName);
                return *map.at(location)->at(noExtensionName);
            }

            template<typename ResourceType>
            static ResourceMultimap<ResourceType>& getMap()
            {
                auto& instance = getInstance();

                if constexpr (std::is_same_v<ResourceType, sf::Texture>) {
                    return instance.m_textures;
                } else if constexpr (std::is_same_v<ResourceType, sf::Font>) {
                    return instance.m_fonts;
                } else if constexpr (std::is_same_v<ResourceType, sf::SoundBuffer>) {
                    return instance.m_soundBuffers;
                } else if constexpr (std::is_same_v<ResourceType, sf::Image>) {
                    return instance.m_images;
                } else if constexpr (std::is_same_v<ResourceType, sf::Music>) {
                    return instance.m_musics;
                } else {
                    THROW("Unknown resource type");
                }
            }

            template<typename T>
            static void assertResourceExist(const std::string& location, const std::string& name)
            {
                auto& instance = getInstance();

                auto& map = getMap<T>();
                if (map.find(location) == map.end()) {
                    // check if the requested Resource is in another ImageMap
                    for (auto& [location, imageMap] : instance.m_images) {
                        if (imageMap->find(name) != imageMap->end()) {
                            THROW("Resource not found: [" + name + "] in [" + location + "] but found in [" + location + "]");
                        }
                    }
                    THROW("Resource not found: [" + name + "] in [" + location + "]");
                }
                if (map.at(location)->find(name) == map.at(location)->end()) {
                    // check if the requested Resource is in another ImageMap
                    for (auto& [location, imageMap] : instance.m_images) {
                        if (imageMap->find(name) != imageMap->end()) {
                            THROW("Resource not found: [" + name + "] in [" + location + "] but found in [" + location + "]");
                        }
                    }
                    THROW("Resource not found: [" + name + "] in [" + location + "]");
                }
            }

            static ResourceManager& getInstance()
            {
                static ResourceManager instance;
                return instance;
            }

            static void updateCompletedTasks()
            {
                auto& instance = getInstance();
                instance.m_completedTasks++;
                instance.m_progressSubject.notify(std::clamp(static_cast<float>(instance.m_completedTasks) / instance.m_totalTasks, 0.0f, 1.0f));
            }

        private:
            std::queue<std::function<bool()>> m_loadingTasks;
            std::vector<std::thread> m_loadingThreads;
            unsigned int m_threadCount;
            std::atomic<bool> m_isLoading;
            std::condition_variable m_condVar;
            std::mutex m_mutex;

            Subject<float> m_progressSubject;
            Subject<bool> m_doneSubject;

            ResourceMultimap<sf::Texture> m_textures;
            ResourceMultimap<sf::Font> m_fonts;
            ResourceMultimap<sf::SoundBuffer> m_soundBuffers;
            ResourceMultimap<sf::Image> m_images;
            ResourceMultimap<sf::Music> m_musics;

            std::atomic<int> m_totalTasks = 0; // New member
            std::atomic<int> m_completedTasks = 0; // New member
    };
}
