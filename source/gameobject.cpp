#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include "gameobject.hpp"
#include "SFMLMath.hpp"
#include "math.hpp"

sf::Texture emptyimage();

//Constructor
Gameobject::Gameobject(std::vector<sf::Sprite*> &collisionList, sf::Vector2<float> _position, float _rotation = 0,sf::Vector2<float> _scale =sf::Vector2<float>(1,1), bool _hasSprite = false, sf::Texture* _texture = nullptr, bool _isStatic = false, bool _hasCollision = true, float _gravity = 10, float _friction = 0,float _bounciness = 0.2, sf::Vector2<float> _startVelocity = sf::Vector2<float>(0,0)) {

    position = _position;
    rotation = _rotation;
    scale = _scale;

    ///Sprite

    hasSprite = _hasSprite;
    if (hasSprite) {

        sprite.setTexture(*_texture);
        sprite.setPosition(position);
        sprite.setRotation(rotation);
        SetScale(_scale);
    }

    velocity = _startVelocity;

    //Physics
    isStatic = _isStatic;
    hasCollision = _hasCollision;
    gravity = _gravity;
    friction = _friction;
    bounciness = _bounciness;
    
    colliding = false;
    drag = 0;


    if (hasCollision && hasSprite) {
        collisionList.push_back(&sprite);
    }

};

//Sets scale of the sprite
void Gameobject::SetScale(sf::Vector2<float> scaleToSet) {
    if (hasSprite)
        sprite.setScale(scaleToSet);
}

//Called before loop after inputs
void Gameobject::OnEvent() {

}

//Called every frame
void Gameobject::OnLoop(sf::Time deltaTime, std::vector<sf::Sprite*> collisionList) {
    if (!isStatic) {
        CalculatePhysics(deltaTime, collisionList);
        //std::cout << "\nRealpos: " << position.x;
    }
};

//Called just before render
void Gameobject::OnRender() {
    if(hasSprite) {
        sprite.setPosition(position);
        sprite.setRotation(rotation);
    }
};

void Gameobject::CalculatePhysics(sf::Time deltaTime, std::vector<sf::Sprite*> collisionList) {

    velocity.y += gravity * deltaTime.asSeconds();

    //Collision detection

    bool test = false;

    for (sf::Sprite* other : collisionList) {
        if (&sprite != other) {
            if (sprite.getGlobalBounds().intersects(other->getGlobalBounds())) {

                sf::FloatRect spriteRect = sprite.getGlobalBounds();
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
                
                float minDistance = INFINITY;
                int side = -1;


                //Top side
                bool sideCollision = topinsideother && !bottominsideother && (leftinsideother || rightinsideother);
                float distance = math::difference(spriteRect.top,otherBottom);
                if (distance < minDistance) {
                    minDistance = distance;
                    if (sideCollision) {
                        side = 0;
                        normal = sf::Vector2<float>(0.f,  1.f);
                    }
                }

                //Right side
                sideCollision = rightinsideother && !leftinsideother && topinsideother && bottominsideother;
                distance = math::difference(spriteRight,otherRect.left);
                if (distance < minDistance) {
                    minDistance = distance;
                    if (sideCollision) {
                        side = 1;
                        normal = sf::Vector2<float>(1.f,  0.f);
                    }
                }
                //Bottom side
                sideCollision = bottominsideother && !topinsideother && (leftinsideother || rightinsideother);
                distance = math::difference(spriteBottom,otherRect.top);
                if (distance < minDistance) {
                    minDistance = distance;
                    if (sideCollision) {
                        side = 2;
                        normal = sf::Vector2<float>(0,  -1.f);
                    }                
                }
                //Left side
                sideCollision = leftinsideother && !rightinsideother && topinsideother && bottominsideother;
                distance = math::difference(spriteRect.left, otherRight);
                if (distance < minDistance) {
                    minDistance = distance;
                    if (sideCollision) {
                        side = 3;
                        normal = sf::Vector2<float>(-1.f,  0.f);
                    }
                }

                //std::cout << "\n Side: " << side << "\n";

                //Move to closest side
                switch (side) {
                    case 0: // Top
                        position.y = otherBottom + spriteRect.height;
                        break;
                    case 1: // Right
                        position.x = otherRect.left - spriteRect.width;
                        break;
                    case 2: // Bottom
                        position.y = otherRect.top - spriteRect.height;
                        break;
                    case 3: // Left
                        position.x = otherRight - spriteRect.width;
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
            }
        }
    }
    colliding = test;

   // std::cout << "\nnewvelo:"<<velocity.y;


    //Scale the velocity to deltaTime to get consistent velocity across framerates
    position += velocity*deltaTime.asSeconds();

};

void Gameobject::SetVelocity(sf::Vector2<float> newVelocity) {
    velocity = newVelocity;
};
sf::Vector2<float> Gameobject::GetVelocity() {
    return velocity;
};