#include <thread>
#include "audio.hpp"


void playSound(sf::SoundBuffer audioBuffer, float volume) {
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