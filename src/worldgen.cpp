#include "worldgen.hpp"
#include "gameobject.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <utility>
#include <vector>
#include "globals.hpp"
#include "items.hpp"
#include "perlinnoise.hpp"
#include "settings.hpp"

//Constructor
tile::tile(sf::String _tileName, float _tileHealth, Gameobject tileObject, std::vector<sf::Texture*> _topOverrideTexture, std::vector<sf::Texture*> _bottomOverrideTexture, bool _dropsItem, treasureItem _itemToDrop) : Gameobject(tileObject) {
    tileName = _tileName;
    maxHealth = _tileHealth;
    health = _tileHealth;
    topOverrideTexture = _topOverrideTexture[rand() % _topOverrideTexture.size()];
    bottomOverrideTexture = _bottomOverrideTexture[rand() % _bottomOverrideTexture.size()];
    dropItem = _dropsItem;
    if (_dropsItem)
        itemToDrop = _itemToDrop;

}
void tile::TakeDamage(float damage) {
    health -= damage;
    if (health <= 0 && !destroyed) {
        destroyed = true;
        //Spawn dropitem
        if (dropItem) {
            treasureItem* dropped = new treasureItem(itemToDrop);
            dropped->resetTexture();
            dropped->velocity = sf::Vector2<float>((((float)rand()/RAND_MAX)-0.5f)*dropped->startVelocity,(((float)rand()/RAND_MAX)-0.5f)*dropped->startVelocity);
            dropped->position.x = position.x+globalsettings.tileSize/2-dropped->sprite.getGlobalBounds().width/2;
            dropped->position.y = position.y+globalsettings.tileSize/2-dropped->sprite.getGlobalBounds().height/2;
            sf::Vector2<int> chunkPos;
            chunkPos.x = dropped->position.x/globalsettings.tileSize/globalsettings.chunkSize;
            chunkPos.y = dropped->position.y/globalsettings.tileSize/globalsettings.chunkSize;

            chunk* _chunk = globalChunkList[chunkPos.x][chunkPos.y];
            _chunk->objects.push_back(dropped);
            _chunk->collisionObjects.push_back(dropped);
            dropped->currentChunk = _chunk;
        }
    }
}



chunk::chunk(sf::Vector2<int> position) {
    chunkPosition = position;
}

void chunk::OnEvents() {
    for (auto& object : objects) {
        object->OnEvent();
    }
}
void chunk::OnLoop(std::vector<chunk*> chunkList, std::vector<std::vector<chunk*>> fullChunkList) {

    int test = 0;

    //Call OnLoop on all objects in chunk
    for (Gameobject* object : objects) {


        if (object->objectName == "Player") return;

        if (object->destroyed) {
            objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());
            if (object->hasCollision) {
                object->hasCollision = false;
                collisionObjects.erase(std::remove(collisionObjects.begin(), collisionObjects.end(), object), collisionObjects.end());
            }
        }

        if (object->objectName == "enemy") 
            std::cout << "Enemy looped " << test << "\n";
        
        test++;


        object->OnLoop(chunkList);
        //Test if object is still in chunk if it is dynamic
        if (!object->isStatic) {
            object->updateCurrentChunk(fullChunkList);
        }
    }
}
void chunk::OnRender() {
    for (auto& object : objects) {
        object->OnRender();
    }
}


level::level(int tileGridSize ,sf::Vector2<int> worldsize, std::vector<tile> tileTypes, int bedrockType, std::map<int, float> noiseTileMap, std::map<int, std::pair<float, int>> secondTileMap) {

    //Generate perlin
    const siv::PerlinNoise::seed_type seed = currentSeed;
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
                currTile.resetTexture();
                column.push_back(currTile);
            }
        }
        tiles.push_back(column);
    }

}
