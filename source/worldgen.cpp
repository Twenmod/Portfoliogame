#include "worldgen.hpp"
#include "gameobject.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <memory>
#include <vector>

//Constructor
tile::tile(sf::String _tileName, float _tileHealth, Gameobject tileObject) : Gameobject(tileObject) {
    tileName = _tileName;
    maxHealth = _tileHealth;
    health = _tileHealth;
}

level::level(int tileGridSize ,sf::Vector2<int> worldsize, std::vector<tile> tileTypes) {
    
    //Spawn all tiles
    tiles = std::vector<std::vector<tile>>(worldsize.x);

    for (int x = 0; x < worldsize.x; x++) {
        tiles[x] =  std::vector<tile>(worldsize.y);
        for (int y = 0; y < worldsize.y; y++) {
            tile currTile = tileTypes[0];
            currTile.position.x = x*tileGridSize;
            currTile.position.y = y*tileGridSize;
            tiles[x][y] = currTile;
        }
    }

}
