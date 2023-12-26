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

        float attackDelay = 0;

        //Constructor
        Player(float walkSpeed, float jumpVelocity, Gameobject playerObject);
        void OnLoop(std::vector<chunk*> chunkList);
        void CalculatePhysics(std::vector<chunk*> chunkList);

        //Attacks in a rect returns true if attack hits something
        bool Attack(sf::FloatRect attackRect, std::vector<chunk*> chunkList, int tileAttackDamage, int enemyAttackDamage);
};


#endif