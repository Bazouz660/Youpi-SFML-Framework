#ifndef YPI_SOUND_MANAGER_HPP
#define YPI_SOUND_MANAGER_HPP

#include <SFML/Audio.hpp>
#include <list>
#include <map>
#include <memory>
#include <algorithm>
#include "../math/numberGenerator.hpp"
#include "../math/Vector2.hpp"
#include "../system/logger.hpp"
#include "../../lib_headers/common.hpp"

namespace ypi {

    class SoundManager {
    private:
        static std::list<std::shared_ptr<sf::Sound>> sounds;
        static std::map<std::string, std::shared_ptr<sf::SoundBuffer>> soundBuffers;
        static const unsigned int maxSounds;

    public:
        static void addSoundBuffer(const std::string& name, const_sptr_ref<sf::SoundBuffer> soundBuffer) {
            soundBuffers[name] = soundBuffer;
        }

        static bool isLoaded(const std::string& name) {
            return soundBuffers.find(name) != soundBuffers.end();
        }

        // Play a sound, returns the sound object
        static std::shared_ptr<sf::Sound> playSound(const std::string& name, bool enablePitchModulation = true, Vector2f pitchRange = Vector2f(0.95f, 1.05f)) {

            // check if the sound is loaded
            if (soundBuffers.find(name) == soundBuffers.end()) {
                logger::error() << "Failed to play sound: " << name << " - sound not loaded";
                return nullptr;
            }

            if (sounds.size() >= maxSounds) {
                // Si nous avons atteint la limite de sons, nous arrêtons le son le plus ancien
                sounds.front()->stop();
                sounds.pop_front();
            }

            std::shared_ptr<sf::Sound> sound = std::make_shared<sf::Sound>();
            sound->setBuffer(*soundBuffers[name]);
            sounds.push_back(sound);

            if (enablePitchModulation)
                sounds.back()->setPitch(nbgen::between(pitchRange.x, pitchRange.y)); // Randomize pitch a bit to avoid repetitive sounds
            else
                sounds.back()->setPitch(1.0f);
            sounds.back()->play();

            return sound;
        }

        static void update() {
            // Supprime les sons qui ont fini de jouer
            sounds.remove_if([](const std::shared_ptr<sf::Sound>& sound) {
                return sound->getStatus() == sf::Sound::Stopped;
            });
        }
    };
}

#endif // !YPI_SOUND_MANAGER_HPP