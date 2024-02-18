
#include "globals.hpp"
#include <SFML/Graphics/Font.hpp>
#include <vector>


std::vector<std::vector<chunk*>> globalChunkList;
int currentSeed;
int exitState = 0;
bool gameRunning;
std::map<std::string, std::vector<sf::SoundBuffer*>> soundmap;
