#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "gameobject.hpp"

class Player : public Gameobject {


    public:
        float walkSpeed;
        float jumpVelocity;
        bool grounded = true;

        //Constructor
        Player(float walkSpeed, float jumpVelocity, Gameobject playerObject);
        void OnLoop(std::vector<sf::Sprite*> collisionList);
        void CalculatePhysics(std::vector<sf::Sprite*> collisionList);
};


#endif