/*
 * @ Author: Basile Trebus--Hamann
 * @ Create Time: 2023-07-15 02:00:21
 * @ Modified by: Basile Trebus--Hamann
 * @ Modified time: 2023-07-15 03:02:48
 * @ Description:
 */

#pragma once

#include "common.hpp"

#include "logger.hpp"
#include "parsing.hpp"
#include "Chrono.hpp"
#include <thread>

namespace exng {

    class MusicManager {
        public:
            static void switchTrack(const std::string& name, float fadeTime, bool loop, bool restart = false);
            static void pauseTrack(float fadeTime);

            static std::string getCurrentTrackName();
            static sf::Music& getCurrentTrack();

            static bool isPlaying(const std::string& name);
            static bool isPaused(const std::string& name);
            static bool isStopped(const std::string& name);

            static void addTrack(const std::string& name, sf::Music* track);

            static void setVolume(float volume);
            static float getMusicVolume();


            static bool isLoaded(const std::string& name);

        private:
            struct SwitchInfo {
                std::string newTrackName;
                float fadeTime;
                bool loop;
                bool restart;
            } m_switchInfo;

            std::thread m_updateThread;
            Chrono m_chrono;
            std::atomic<bool> m_isRunning;

            MusicManager();

            ~MusicManager();

            static MusicManager& getInstance()
            {
                static MusicManager instance;
                return instance;
            }

            static void update(float dt);
            static void fadeIn(float fadeTime, float dt);
            static void fadeOut(float fadeTime, float dt);

            float m_fadeTimer = 0.f;
            bool m_fadingOut = false;
            bool m_fadingIn = false;
            bool m_switching = false;

            float m_musicVolume = 100.0f;

            std::map<std::string, sf::Music*> m_musicMap;
            sf::Music* m_currentTrack = nullptr;
    };

}