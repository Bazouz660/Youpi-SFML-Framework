/*
 * @ Author: Basile Trebus--Hamann
 * @ Create Time: 2023-07-15 02:09:27
 * @ Modified by: Basile Trebus--Hamann
 * @ Modified time: 2023-07-15 16:15:54
 * @ Description:
 */

#include "MusicManager.hpp"

namespace exng {

    MusicManager::MusicManager() : m_isRunning(true)
    {
        m_updateThread = std::thread([this]() {
            while (m_isRunning) {
                m_chrono.update();
                update(m_chrono.getFrameDt().asSeconds());
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }

    MusicManager::~MusicManager()
    {
        m_isRunning = false;
        if (m_updateThread.joinable()) {
            m_updateThread.join();
        }
    }

    void MusicManager::fadeIn(float fadeTime, float dt)
    {
        MusicManager& inst = getInstance();

        if (inst.m_currentTrack == nullptr) {
            return;
        }

        inst.m_fadeTimer += dt;

        float volume = inst.m_fadeTimer / fadeTime * inst.m_musicVolume;
        volume = std::clamp(volume, 0.0f, inst.m_musicVolume);
        inst.m_currentTrack->setVolume(volume);

        if (inst.m_fadeTimer >= fadeTime) {
            inst.m_fadingIn = false;
            inst.m_fadeTimer = 0.0f;
        }
    }

    void MusicManager::fadeOut(float fadeTime, float dt)
    {
        MusicManager& inst = getInstance();

        if (inst.m_currentTrack == nullptr) {
            return;
        }

        inst.m_fadeTimer += dt;

        float volume = inst.m_fadeTimer / fadeTime * inst.m_musicVolume;
        volume = std::clamp(volume, 0.0f, inst.m_musicVolume);
        inst.m_currentTrack->setVolume(inst.m_musicVolume - volume);

        if (inst.m_fadeTimer >= fadeTime) {
            inst.m_fadingOut = false;
            inst.m_fadeTimer = 0.0f;

            if (inst.m_switchInfo.newTrackName == "") {
                inst.m_currentTrack->pause();
                inst.m_currentTrack = nullptr;
                return;
            }

            inst.m_currentTrack->pause();
            inst.m_currentTrack = inst.m_musicMap[inst.m_switchInfo.newTrackName];
            inst.m_currentTrack->play();
            inst.m_currentTrack->setLoop(inst.m_switchInfo.loop);
            if (inst.m_switchInfo.restart) {
                inst.m_currentTrack->setPlayingOffset(sf::Time::Zero);
            }
        }
    }

    void MusicManager::update(float dt)
    {
        // update fading
        MusicManager& inst = getInstance();

        if (inst.m_switching) {
            if (inst.m_fadingOut)
                fadeOut(inst.m_switchInfo.fadeTime, dt);
            else if (inst.m_fadingIn)
                fadeIn(inst.m_switchInfo.fadeTime, dt);
            else
                inst.m_switching = false;
        }

    }

    void MusicManager::setVolume(float volume)
    {
        MusicManager& inst = getInstance();

        inst.m_musicVolume = volume;
        if (inst.m_currentTrack != nullptr) {
            inst.m_currentTrack->setVolume(volume);
        }
    }

    void MusicManager::pauseTrack(float fadeTime)
    {
        MusicManager& inst = getInstance();

        if (inst.m_currentTrack == nullptr) {
            return;
        }

        inst.m_switchInfo = { "", fadeTime, inst.m_currentTrack->getLoop(), false };

        inst.m_fadingOut = true;
        inst.m_fadingIn = false;
        inst.m_switching = true;
    }

    // switch track with fade in/out (gradually change volume)
    void MusicManager::switchTrack(const std::string& name, float fadeTime, bool loop, bool restart)
    {
        MusicManager& inst = getInstance();

        if (inst.m_musicMap.find(name) == inst.m_musicMap.end()) {
            logger::error("MusicManager::switchTrack: music not found: " + name);
            return;
        }

        inst.m_switchInfo = { name, fadeTime, loop, restart };

        if (inst.m_currentTrack == nullptr) {
            inst.m_currentTrack = inst.m_musicMap[name];
            inst.m_currentTrack->play();
            inst.m_currentTrack->setLoop(loop);
            inst.m_fadingIn = true;
            inst.m_fadingOut = false;
            inst.m_switching = true;
            return;
        }

        if (inst.m_currentTrack == inst.m_musicMap[name]) {
            if (restart) {
                inst.m_currentTrack->setPlayingOffset(sf::Time::Zero);
                inst.m_currentTrack->setLoop(loop);
            }
            return;
        }

        inst.m_fadingOut = true;
        inst.m_fadingIn = false;
        inst.m_switching = true;
    }

    std::string MusicManager::getCurrentTrackName()
    {
        MusicManager& inst = getInstance();

        if (inst.m_currentTrack == nullptr) {
            return "";
        }

        for (auto& [name, music] : inst.m_musicMap) {
            if (inst.m_currentTrack == music) {
                return name;
            }
        }

        return "";
    }

    sf::Music& MusicManager::getCurrentTrack()
    {
        MusicManager& inst = getInstance();

        if (inst.m_currentTrack == nullptr) {
            logger::error("MusicManager::getCurrentTrack: no current track");
            throw std::runtime_error("MusicManager::getCurrentTrack: no current track");
        }

        return *inst.m_currentTrack;
    }

    bool MusicManager::isPlaying(const std::string& name)
    {
        MusicManager& inst = getInstance();

        if (inst.m_musicMap.find(name) == inst.m_musicMap.end()) {
            logger::error("MusicManager::isPlaying: music not found: " + name);
            return false;
        }

        return inst.m_musicMap[name]->getStatus() == sf::Music::Playing;
    }

    bool MusicManager::isPaused(const std::string& name)
    {
        MusicManager& inst = getInstance();

        if (inst.m_musicMap.find(name) == inst.m_musicMap.end()) {
            logger::error("MusicManager::isPaused: music not found: " + name);
            return false;
        }

        return inst.m_musicMap[name]->getStatus() == sf::Music::Paused;
    }

    bool MusicManager::isStopped(const std::string& name)
    {
        MusicManager& inst = getInstance();

        if (inst.m_musicMap.find(name) == inst.m_musicMap.end()) {
            logger::error("MusicManager::isStopped: music not found: " + name);
            return false;
        }

        return inst.m_musicMap[name]->getStatus() == sf::Music::Stopped;
    }

    void MusicManager::addTrack(const std::string& name, sf::Music* track)
    {
        MusicManager& inst = getInstance();

        if (inst.m_musicMap.find(name) != inst.m_musicMap.end()) {
            logger::error("MusicManager::addTrack: music already exists: " + name);
            return;
        }

        inst.m_musicMap[name] = track;
    }

    bool MusicManager::isLoaded(const std::string& name)
    {
        MusicManager& inst = getInstance();

        return inst.m_musicMap.find(name) != inst.m_musicMap.end();
    }

    float MusicManager::getMusicVolume()
    {
        MusicManager& inst = getInstance();

        return inst.m_musicVolume;
    }

}