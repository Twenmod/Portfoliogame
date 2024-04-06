#ifndef  AUDIO_H
#define AUDIO_H

#include <SFML/Audio.hpp>

//Function that plays a soundbuffer and cleans up after, Make sure to run this in another thread
void playSound(sf::SoundBuffer audioBuffer, float volume); // Non positional
void playSound(sf::SoundBuffer audioBuffer, float volume, sf::Vector2f position, sf::Vector2f listenerposition); // Positional

#endif