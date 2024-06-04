#include "SoundManager.hpp"

std::list<std::shared_ptr<sf::Sound>> ypi::SoundManager::sounds;
std::map<std::string, std::shared_ptr<sf::SoundBuffer>> ypi::SoundManager::soundBuffers;
const unsigned int ypi::SoundManager::maxSounds = 100;