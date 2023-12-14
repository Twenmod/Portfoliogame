#ifndef WORLDGEN_H
#define WORLDGEN_H
#include "gameobject.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <iostream>
#include <vector>

class tile : public Gameobject {

    public:
        sf::String tileName;
        float health;
        float maxHealth;
        //Constructor
        tile(sf::String _tileName,float _tileHealth, Gameobject tileObject);

};

class level {

    public:
        std::vector<std::vector<tile>> tiles;

        level(int tileGridSize ,sf::Vector2<int> worldsize, std::vector<tile> tileTypes,std::map<int, float> noiseTileMap);


};





#endif