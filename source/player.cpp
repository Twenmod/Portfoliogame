#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include "player.hpp"
#include "gameobject.hpp"
#include "SFMLMath.hpp"
#include "settings.hpp"
#include "math.hpp"
#include "worldgen.hpp"

//Constructor
Player::Player(float _walkSpeed, float _jumpVelocity, Gameobject playerObject) : Gameobject(playerObject) {
    walkSpeed = _walkSpeed;
    jumpVelocity = _jumpVelocity;
}

void Player::OnLoop(std::vector<chunk*> chunkList) {
    sf::Vector2<float> velocity = Gameobject::GetVelocity();
    
    float acceleration = globalsettings.playerAcceleration * deltaTime.asSeconds();
    if (!grounded) acceleration *= globalsettings.playerAirAccelerationMultiplier; 

    if (sf::Keyboard::isKeyPressed(globalsettings.right)) {
        velocity.x += acceleration;
        if (velocity.x > walkSpeed)
            velocity.x = walkSpeed;
    }
    else if (sf::Keyboard::isKeyPressed(globalsettings.left)) {
        velocity.x -= acceleration;
        if (velocity.x < -walkSpeed)
            velocity.x = -walkSpeed;
    }else {
        acceleration = globalsettings.playerStopAcceleration;
        if (!grounded)
            acceleration *= globalsettings.playerAirAccelerationMultiplier;
        
        acceleration = globalsettings.playerStopAcceleration*deltaTime.asSeconds();
        if (velocity.x > acceleration)
            velocity.x -= acceleration;
        else if (velocity.x < -acceleration)
            velocity.x += acceleration;
        else velocity.x = 0;
    }

    if (sf::Keyboard::isKeyPressed(globalsettings.jump) && grounded) {
        velocity.y = -jumpVelocity;
        grounded = false;
    }

    //Attacks
    attackDelay -= deltaTime.asSeconds();

    if (sf::Keyboard::isKeyPressed(globalsettings.attackRight) && attackDelay <= 0) {
        attackDelay = globalsettings.attackInterval;
        sf::FloatRect attackRect(sprite.getScale().x,8,globalsettings.attackRange,16);
        Attack(attackRect,chunkList,globalsettings.attackDamage,globalsettings.attackDamage);
    }
    if (sf::Keyboard::isKeyPressed(globalsettings.attackLeft) && attackDelay <= 0) {
        attackDelay = globalsettings.attackInterval;
        sf::FloatRect attackRect(-globalsettings.attackRange,8,globalsettings.attackRange,16);
        Attack(attackRect,chunkList,globalsettings.attackDamage,globalsettings.attackDamage);
    }
    if (sf::Keyboard::isKeyPressed(globalsettings.attackUp) && attackDelay <= 0) {
        attackDelay = globalsettings.attackInterval;
        sf::FloatRect attackRect(8,-globalsettings.attackRange,16,globalsettings.attackRange);
        Attack(attackRect,chunkList,globalsettings.attackDamage,globalsettings.attackDamage);
    }
    if (sf::Keyboard::isKeyPressed(globalsettings.attackDown) && attackDelay <= 0) {
        attackDelay = globalsettings.attackInterval;
        sf::FloatRect attackRect(8,sprite.getScale().y,16,globalsettings.attackRange);
        Attack(attackRect,chunkList,globalsettings.attackDamage,globalsettings.attackDamage);
    }

    Gameobject::SetVelocity(velocity);





    //Calculate Base gameobject physics 
    Gameobject::OnLoop(chunkList);
}

void Player::CalculatePhysics(std::vector<chunk*> chunkList) {

    //Scale the velocity to deltaTime to get consistent velocity across framerates

    float currentGravity = gravity;
    if(sf::Keyboard::isKeyPressed(globalsettings.jump) && velocity.y < 0)
        currentGravity = globalsettings.jumpGravity;

    velocity.y += currentGravity * deltaTime.asSeconds();

    position += velocity*deltaTime.asSeconds();

    //Collision detection
    bool test = false;

    sf::Vector2<float> scaledVelocity = velocity * deltaTime.asSeconds();
    float distance = std::sqrt(scaledVelocity.x * scaledVelocity.x + scaledVelocity.y * scaledVelocity.y);
    int requiredIterations = std::clamp((int)std::round(distance / globalsettings.playerPhysicsStepDistance),1,globalsettings.playerMaxPhysicsIterations);
    sf::Vector2<float> step = sf::Vector2<float>(scaledVelocity.x / requiredIterations, scaledVelocity.y / requiredIterations);

    for (int i = 0; i < requiredIterations; i++) {

        if (test == true)
            break;

        sf::Sprite _sprite = sprite;
        _sprite.setPosition(position + sf::Vector2<float>(step.x * i, step.y * i));
        sf::FloatRect spriteRect = _sprite.getGlobalBounds();

        for (chunk* _chunk : chunkList) {
            if (_chunk->collisionObjects.size() == 0) {
                continue;
            }
            for (Gameobject* otherobject : _chunk->collisionObjects) 
            {
                sf::Sprite* other = &otherobject->sprite;
                if (&sprite != other) 
                {
                    if (spriteRect.intersects(other->getGlobalBounds())) {

                        sf::FloatRect otherRect = other->getGlobalBounds();


                        sf::Vector2<float> relativeVelocity = velocity;

                        sf::Vector2<float> normal(0,0);

                        //Simplify rect sides
                        float spriteBottom = spriteRect.top + spriteRect.height;
                        float otherBottom = otherRect.top + otherRect.height;
                        float spriteRight = spriteRect.left + spriteRect.width;
                        float otherRight = otherRect.left + otherRect.width;

                        //Test all sides
                        bool bottominsideother = spriteBottom <= otherBottom && spriteBottom >= otherRect.top;
                        bool topinsideother = spriteRect.top >= otherRect.top && spriteRect.top <= otherBottom;
                        bool leftinsideother = spriteRect.left >= otherRect.left && spriteRect.left <= otherRight;
                        bool rightinsideother = spriteRight >= otherRect.left && spriteRight <= otherRight;



                        //Find side by checking smallest distance between the sides
                        
                        float groundedMinDistance = 1.f; // Override distance for bottom side to make sure player does not collide with side of tiles when walking on them

                        float minDistance = INFINITY;
                        int side = -1;


                        float topDistance = math::difference(spriteRect.top,otherBottom); // Top
                        if (topDistance < minDistance) {
                            minDistance = topDistance;
                        }
                        float rightDistance = math::difference(spriteRight,otherRect.left); // Right
                        if (rightDistance < minDistance) {
                            minDistance = rightDistance;
                        }
                        float bottomDistance = math::difference(spriteBottom,otherRect.top); // Bottom
                        if (bottomDistance < minDistance) {
                            minDistance = bottomDistance;              
                        }
                        float leftDistance = math::difference(spriteRect.left, otherRight); // Left
                        if (leftDistance < minDistance) {
                            minDistance = leftDistance;
                        }

                        //Top side
                        bool sideCollision = topinsideother && !bottominsideother && (leftinsideother || rightinsideother);
                        if (sideCollision && topDistance == minDistance) {
                            side = 0;
                            normal = sf::Vector2<float>(0.f,  1.f);
                        }
                        //Right side
                        sideCollision = rightinsideother && !leftinsideother && (topinsideother || bottominsideother);
                        if (sideCollision && rightDistance == minDistance) {
                            side = 1;
                            normal = sf::Vector2<float>(1.f,  0.f);
                        }
                        //Left side
                        sideCollision = leftinsideother && !rightinsideother && (topinsideother || bottominsideother);
                        if (sideCollision && leftDistance == minDistance) {
                            side = 3;
                            normal = sf::Vector2<float>(-1.f,  0.f);
                        }
                        //Bottom side
                        sideCollision = bottominsideother && !topinsideother && (leftinsideother || rightinsideother);
                        if (sideCollision && (bottomDistance <= groundedMinDistance || bottomDistance == minDistance)) {
                            side = 2;
                            normal = sf::Vector2<float>(0,  -1.f);
                            grounded = true;
                        } else {
                            grounded = false;
                        }

                        //Move to closest side
                        switch (side) {
                            case 0: // Top
                                position.y = otherBottom;
                                if (velocity.y < 0) velocity.y = 0;
                                test = true;
                                break;
                            case 1: // Right
                                position.x = otherRect.left - spriteRect.width;
                                if (velocity.x > 0) velocity.x = 0;
                                test = true;
                                break;
                            case 2: // Bottom
                                position.y = otherRect.top - spriteRect.height;
                                if(velocity.y > 0) velocity.y = 0;
                                test = true;
                                break;
                            case 3: // Left
                                position.x = otherRight;
                                if (velocity.x < 0) velocity.x = 0;
                                test = true;
                                break;
                        }
                    }
                }
            }
        }
    }
    colliding = test;
};

bool Player::Attack(sf::FloatRect attackRect, std::vector<chunk*> chunkList, int tileAttackDamage, int enemyAttackDamage) {
    //Construct attackRect
    attackRect.left += position.x;
    attackRect.top += position.y;

    //Get all colliding objects
    std::vector<Gameobject*> collidingObjects;
    for (chunk* chunk : chunkList) {
        for (Gameobject* gameobject : chunk->objects) {
            if (gameobject->hasCollision) {
                if (attackRect.intersects(gameobject->sprite.getGlobalBounds())) {
                    collidingObjects.push_back(gameobject);
                }
            }
        }
    }

    if (collidingObjects.size() != 0) {
        bool hitSomething = false;

        for (Gameobject* gameobject : collidingObjects) {
            //Check if object is tile
            if (dynamic_cast<tile*>(gameobject)) {
                gameobject->TakeDamage(tileAttackDamage);
                hitSomething = true;
            }
        }

        return hitSomething;
    }
    return false;
}


