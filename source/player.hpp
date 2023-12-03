#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "gameobject.hpp"

class Player : public Gameobject {
    float walkSpeed;
    float jumpVelocity;
    

    public:
        //Constructor
        Player(float walkSpeed, float jumpVelocity, Gameobject playerObject);
        void OnLoop(sf::Time deltaTime, std::vector<sf::Sprite*> collisionList);

};


#endif