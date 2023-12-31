#ifndef WORLDGEN_H
#define WORLDGEN_H
#include "gameobject.hpp"
#include "items.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>


class tile : public Gameobject {

    public:
        sf::String tileName;
        float health;
        float maxHealth;
        //Constructor
        tile(sf::String _tileName,float _tileHealth, Gameobject tileObject, std::vector<sf::Texture*> _topOverrideTexture, std::vector<sf::Texture*> _bottomOverrideTexture,bool dropsItem, treasureItem _itemToDrop);
        void TakeDamage(float damage);
        sf::Texture* topOverrideTexture;
        sf::Texture* bottomOverrideTexture;
        treasureItem itemToDrop = treasureItem();
    private:
        bool dropItem = false;

};

class chunk {
    public:
        chunk(sf::Vector2<int> position);
        std::vector<Gameobject*> objects;
        std::vector<Gameobject*> collisionObjects;
        
        sf::Vector2<int> chunkPosition;
        //Call OnEvent on all objects in chunk
        void OnEvents();
        //Call OnLoop on all objects in chunk
        void OnLoop(std::vector<chunk*> chunkList, std::vector<std::vector<chunk*>> fullChunkList);
        //Call OnRender on all objects in chunk
        void OnRender();
};

class level {

    public:
        std::vector<std::vector<tile>> tiles;

        level(int tileGridSize ,sf::Vector2<int> worldsize, std::vector<tile> tileTypes, int bedrockType, std::map<int, float> noiseTileMap, std::map<int, std::pair<float, int>> secondTileMap);


};





#endif