#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "gameobject.hpp"

class Player : public Gameobject {


    public:

        int gold = 0;

        float health = 10;
        float maxHealth = 10;

        float walkSpeed;
        float jumpVelocity;
        bool grounded = true;
        sf::FloatRect playerRect;

        sf::Vector2<float> spriteOffset;

        //Lower values mean faster animation
        float animationWalkSpeed = 9;

        //Constructor
        Player(float walkSpeed, float jumpVelocity, sf::FloatRect playerRect, sf::Vector2<float> _size, sf::Vector2<float> spriteOffset, Gameobject playerObject);
        void OnLoop(std::vector<chunk*> chunkList);
        void CalculatePhysics(std::vector<chunk*> chunkList);
        void OnRender();

        //Attacks in a rect returns true if attack hits something
        bool Attack(sf::FloatRect attackRect, std::vector<chunk*> chunkList, int tileAttackDamage, int enemyAttackDamage);

        
    private:

        bool jumpKeyHold = false;
        bool jumpKeyDown = false;

        //Animation/Sprites

        float cayote;
        bool wasGrounded;
        bool attacking;
        int attackDirection; // 0 is left 1 is right 2 is up 3 is down
        float attackInterval = 0;
        float _attackDelay;


        //Direction player is facing 0 is left 1 is right
        bool facing;
        bool walking;
        bool jumpTrigger;

        float walkanimationDelay = 0;
        int walkanimationFrame = 0;

        float animationDelay;
};


#endif