#ifndef  AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>

//Function that plays a soundbuffer and cleans up after, Make sure to run this in another thread
void playSound(sf::SoundBuffer audioBuffer, float volume);

#endif