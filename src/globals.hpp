#ifndef GLOBALS_H
#define GLOBALS_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class chunk;

extern std::vector<std::vector<chunk*>> globalChunkList;
extern int currentSeed;
extern int exitState;
extern bool gameRunning;
extern std::map<std::string, std::vector<sf::SoundBuffer*>> soundmap;

#endif