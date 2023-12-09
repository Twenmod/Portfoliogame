#include "worldgen.hpp"
#include "gameobject.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include "perlinnoise.hpp"


//Constructor
tile::tile(sf::String _tileName, float _tileHealth, Gameobject tileObject) : Gameobject(tileObject) {
    tileName = _tileName;
    maxHealth = _tileHealth;
    health = _tileHealth;
}

level::level(int tileGridSize ,sf::Vector2<int> worldsize, std::vector<tile> tileTypes) {
    
    //Generate perlin
    const siv::PerlinNoise::seed_type seed = random();
    const siv::PerlinNoise perlin { seed };

    



    //Spawn all tiles
    for (int x = 0; x < worldsize.x; x++) {
        std::vector<tile> column;
        for (int y = 0; y < worldsize.y; y++) {
            tile currTile = tileTypes[0];
            currTile.position.x = x*tileGridSize;
            currTile.position.y = y*tileGridSize;
            column.push_back(currTile);
            std::cout <<"spawning tile at x: " << x << " y: " << y << std::endl;
        }
        tiles.push_back(column);
    }

}
