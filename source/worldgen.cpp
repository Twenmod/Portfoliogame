#include "worldgen.hpp"
#include "gameobject.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include "perlinnoise.hpp"
#include "settings.hpp"


//Constructor
tile::tile(sf::String _tileName, float _tileHealth, Gameobject tileObject) : Gameobject(tileObject) {
    tileName = _tileName;
    maxHealth = _tileHealth;
    health = _tileHealth;
}

chunk::chunk(sf::Vector2<int> position) {
    chunkPosition = position;
}

void chunk::OnEvents() {
    for (auto& object : objects) {
        object->OnEvent();
    }
}
void chunk::OnLoop(std::vector<chunk*> chunkList) {
    //Call OnLoop on all objects in chunk
    for (auto& object : objects) {
        object->OnLoop(chunkList);
    }
}
void chunk::OnRender() {
    for (auto& object : objects) {
        object->OnRender();
    }
}


level::level(int tileGridSize ,sf::Vector2<int> worldsize, std::vector<tile> tileTypes, int bedrockType, std::map<int, float> noiseTileMap, std::map<int, std::pair<float, int>> secondTileMap) {

    //Generate perlin
    const siv::PerlinNoise::seed_type seed = time(0);
    const siv::PerlinNoise perlin { seed };

    sf::Vector2<int> tileWorldSize = worldsize * globalsettings.chunkSize;


    //Spawn all tiles
    for (int x = 0; x < tileWorldSize.x; x++) {
        std::vector<tile> column;
        for (int y = 0; y < tileWorldSize.y; y++) {
            int type = -1;
            //Check if tile is on edge
            if (x == 0 || y == 0 || x == tileWorldSize.x-1 || y == tileWorldSize.y-1) {
                type = bedrockType;
            }else {
                //Get noise
                const double noise = perlin.octave2D_01((x * globalsettings.noiseScale.x), (y * globalsettings.noiseScale.y), globalsettings.octaves, globalsettings.persistence);
                const double secondNoise = perlin.octave2D_01((x+1000)*globalsettings.oreNoiseScale.x, (y+1000)*globalsettings.oreNoiseScale.y, globalsettings.octaves, globalsettings.persistence);

                for(auto const& mapLocation : noiseTileMap) {
                    if (noise > mapLocation.second) { // If noise value is falls bellow location
                        type = mapLocation.first; // Set the type to type of that map location
                        auto const& replacementMap = secondTileMap.find(mapLocation.first);
                        if (type == replacementMap->first && replacementMap != secondTileMap.end()) {
                            if (secondNoise > replacementMap->second.first) {
                                type = replacementMap->second.second;
                            }
                        }
                    }
                }
            }
            //Spawn corresponding tile
            if (type != -1) {
                tile currTile = tileTypes[type];
                currTile.position.x = x*tileGridSize;
                currTile.position.y = y*tileGridSize;
                column.push_back(currTile);
                std::cout <<"spawning tile at x: " << x << " y: " << y << std::endl;
            }
        }
        tiles.push_back(column);
    }

    //Spawn enemies
    int amountToSpawn = random() % globalsettings.amountOfEnemies.y + globalsettings.amountOfEnemies.x;
    for (int i = 0; i < amountToSpawn; i++) {
        bool spawned = false; 
        while (!spawned) {
            int x = random() % tileWorldSize.x;
            int y = random() % tileWorldSize.y;
            //Check if tile position is empty
            if (tiles[x][y].tileName == "Air") {
                //Spawn enemy
                std::cout << "spawning enemy at x: " << x << " y: " << y << std::endl;
                spawned = true;
            }
        }
    }

}
