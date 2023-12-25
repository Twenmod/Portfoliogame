#include <iostream>
#include "enemy.hpp"
#include <algorithm>
#include <vector>
#include "math.hpp"
#include "gameobject.hpp"
#include "SFMLMath.hpp"
#include "settings.hpp"
#include "worldgen.hpp"

Enemy::Enemy(float _health, float _speed, Gameobject enemyObject) : Gameobject(enemyObject) {
    health = _health;
    speed = _speed;
    moveDirection = 1;
};

void Enemy::CalculatePhysics(std::vector<chunk*> chunkList) 
{

    if (moveDirection == 1) {
        velocity.x = speed;
    }else {
        velocity.x = -speed;
    }

    velocity.y += gravity * deltaTime.asSeconds();

    //Scale the velocity to deltaTime to get consistent velocity across framerates
    position += velocity*deltaTime.asSeconds();

    //Collision detection

    bool test = false;

    for (chunk* _chunk : chunkList) 
    {
        if (_chunk->collisionObjects.size() == 0) continue;
        for (sf::Sprite* other : _chunk->collisionObjects) 
        {
            if (&sprite != other) {
                if (sprite.getGlobalBounds().intersects(other->getGlobalBounds())) {
                    sf::Sprite _sprite = sprite;
                    _sprite.setPosition(position);
                    sf::FloatRect spriteRect = _sprite.getGlobalBounds();
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

                    /*Debug visualizer*//*
                    std::cout << "\n  T:"<< topinsideother << "\n"
                    << leftinsideother << " o " << rightinsideother << "\n"
                    << "  B:" << bottominsideother << "\n"; 
                    //*/

                    //Find side by checking smallest distance between the sides
                    

                    //Find side by checking smallest distance between the sides
                    
                    float groundedMinDistance = 0.5f; // Override distance for bottom side to make sure enemy does not collide with side of tiles when walking on them

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
                    if (sideCollision && bottomDistance == minDistance || bottomDistance <= groundedMinDistance) {
                        side = 2;
                        normal = sf::Vector2<float>(0,  -1.f);
                    }

                    //std::cout << "\n Side: " << side << "\n";

                    //Move to closest side
                    switch (side) {
                        case 0: // Top
                            position.y = otherBottom;
                            break;
                        case 1: // Right
                            position.x = otherRect.left - spriteRect.width;
                            break;
                        case 2: // Bottom
                            position.y = otherRect.top - spriteRect.height;
                            break;
                        case 3: // Left
                            position.x = otherRight;
                            break;
                    }

                    //Negate velocity / bounce
                    float totalVelocity = -1*(normal*relativeVelocity);
                    //std::cout << "\nOldvelo:"<<velocity.y<<"\nAddVelo: " << totalVelocity;

                    //Apply velocity;
                    bool normalNegative = normal.y < 0 || normal.x < 0;
                    bool addNegative = totalVelocity < 0;
                    if ((normalNegative && !addNegative) || (addNegative && !normalNegative))
                        velocity += normal * totalVelocity;

                    //std::cout << "\n Normal: " << normal.y;
                    if (normal != sf::Vector2<float>(0,0)) {
                        velocity += -relativeVelocity*friction;
                        test = true;
                    }

                    if (side == 1) {
                        moveDirection = 0;
                    }else if (side == 3) {
                        moveDirection = 1;
                    }
                }
            }
        }
    }
    colliding = test;

    // std::cout << "\nnewvelo:"<<velocity.y;

};