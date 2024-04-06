#include <thread>
#include "audio.hpp"
#include "globals.hpp"
#include "settings.hpp"

void playSound(sf::SoundBuffer audioBuffer, float volume) {
    if (!soundEnabled) return;

    sf::Sound* sound = new sf::Sound;
    sound->setBuffer(audioBuffer);
    sound->setVolume(volume);
    sound->play();

    //Wait till finished playing
    while (sound->getStatus() == sf::Sound::Playing) {
        //Check every couple ms
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    delete sound;
}

void playSound(sf::SoundBuffer audioBuffer, float volume, sf::Vector2f position, sf::Vector2f listenerposition) {
    if (!soundEnabled) return;

    //Calculate volume
    sf::Vector2f diff = position - listenerposition;
    float distance = diff.x * diff.x + diff.y * diff.y; //Normally take the sqrt from this value however we do not to make it drop off exponentially
    float volumeMultiplier = 1 / (distance * globalsettings.audioFalloff);


    sf::Sound* sound = new sf::Sound;
    sound->setBuffer(audioBuffer);
    sound->setVolume(volume*volumeMultiplier);
    sound->play();

    //Wait till finished playing
    while (sound->getStatus() == sf::Sound::Playing) {
        //Check every couple ms
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    delete sound;
}