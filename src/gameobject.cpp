#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <vector>
#include "gameobject.hpp"
#include "SFMLMath.hpp"
#include "math.hpp"
#include "settings.hpp"
#include "worldgen.hpp"
#include "globals.hpp"

sf::Texture emptyimage();

//Constructor
Gameobject::Gameobject(sf::Vector2<float> _position, float _rotation,sf::Vector2<float> _size, bool _hasSprite, std::vector<sf::Texture*> _texture, bool _isStatic, bool  _hasCollision, float _gravity, float _friction, float _bounciness, float _impactDamage, sf::Vector2<float> _startVelocity, sf::String _objectName) {

    position = _position;
    rotation = _rotation;

    objectName = _objectName;
    

    ///Sprite

    hasSprite = _hasSprite;
    if (hasSprite) {
        textures = _texture;

        int select = rand() % _texture.size();
        sf::Texture* selectedTexture = _texture[select];
        sf::Vector2u imageSize = selectedTexture->getSize();
        scale.x = 1.f / imageSize.x * _size.x;
        scale.y = 1.f / imageSize.y * _size.y;
        sprite.setTexture(*selectedTexture);
        sprite.setPosition(position);
        sprite.setRotation(rotation);
        SetScale(scale);
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

    impactDamage = _impactDamage;

    currentChunk = nullptr;

};

Gameobject::~Gameobject() {

    //Remove itself from chunk
    if (currentChunk != nullptr) {
        currentChunk->objects.erase(std::remove(currentChunk->objects.begin(), currentChunk->objects.end(), this), currentChunk->objects.end());

        //Also remove from collision if applicable
        if (hasCollision) currentChunk->collisionObjects.erase(std::remove(currentChunk->collisionObjects.begin(), currentChunk->collisionObjects.end(), this), currentChunk->collisionObjects.end());
    }
}

void Gameobject::resetTexture() {
    if (hasSprite) {
        int select = rand() % textures.size();
        sf::Texture* selectedTexture = textures[select];

        sprite.setTexture(*selectedTexture);
    }
}

//Sets scale of the sprite
void Gameobject::SetScale(sf::Vector2<float> scaleToSet) {
    if (hasSprite)
        sprite.setScale(scaleToSet);
}

//Called before loop after inputs
void Gameobject::OnEvent() {

}

//Called every frame
void Gameobject::OnLoop(std::vector<chunk*> chunkList) {
    if (!isStatic) {
        CalculatePhysics(chunkList);
    }
};

void Gameobject::updateCurrentChunk(std::vector<std::vector<chunk*>> chunkList) {
    //check if still in active chunk
    sf::Vector2<int> chunkPos;
    chunkPos.x = (int)(position.x/globalsettings.tileSize/globalsettings.chunkSize);
    chunkPos.y = (int)(position.y/globalsettings.tileSize/globalsettings.chunkSize);

    if (currentChunk != nullptr) {
        if (chunkPos*globalsettings.chunkSize != currentChunk->chunkPosition) { // If isnt in current chunk
            //Check if still in world
            if (((chunkPos.x < 0 || chunkPos.x >= globalsettings.worldSize.x) || (chunkPos.y <= 0 || chunkPos.y >= globalsettings.worldSize.y))) {
                return;
            }

            //Remove from current Chunk
            currentChunk->objects.erase(std::remove(currentChunk->objects.begin(), currentChunk->objects.end(), this), currentChunk->objects.end());
            if (hasCollision) {
                currentChunk->collisionObjects.erase(std::remove(currentChunk->collisionObjects.begin(), currentChunk->collisionObjects.end(), this), currentChunk->collisionObjects.end());
            }
            //Set to new chunk
            currentChunk = chunkList[chunkPos.x][chunkPos.y];;
            currentChunk->objects.push_back(this);
            if (hasCollision) {
                currentChunk->collisionObjects.push_back(this);
            }
        }
    }else {
            if (((chunkPos.x < 0 || chunkPos.x >= globalsettings.worldSize.x) || (chunkPos.y <= 0 || chunkPos.y >= globalsettings.worldSize.y))) {
                return;
            }
            currentChunk = chunkList[chunkPos.x][chunkPos.y];;
            currentChunk->objects.push_back(this);
            if (hasCollision) {
                currentChunk->collisionObjects.push_back(this);
            }
    }
}

//Called just before render
void Gameobject::OnRender() {
    if(hasSprite) {
        sprite.setPosition(position);
        sprite.setRotation(rotation);
    }
};

void Gameobject::CalculatePhysics(std::vector<chunk*> chunkList) {

    velocity.y += gravity * deltaTime.asSeconds();

    //Collision detection

    bool test = false;

    for (chunk* _chunk : chunkList) 
    {
        if (_chunk->collisionObjects.size() == 0) continue;
        for (Gameobject* otherobject : _chunk->collisionObjects) 
        {
            if (!otherobject->hasCollision) {
                continue;
            }
            
            sf::Sprite* other = &otherobject->sprite;
            if (&sprite == other) continue;

            if (sprite.getGlobalBounds().intersects(other->getGlobalBounds())) 
            {


                sf::Sprite _sprite = sprite;
                _sprite.setPosition(position);
                sf::FloatRect spriteRect = _sprite.getGlobalBounds();
                sf::FloatRect otherRect = other->getGlobalBounds();

                //Use correct rect when colliding with player
                if (Player* playerobject = dynamic_cast<Player*>(other)) 
                {
                    otherRect = playerobject->playerRect;
                    //make sure still collides otherwise continue to next object
                    if (!spriteRect.intersects(otherRect)) {
                        continue;
                    }
                }

                //Not actually relative but good aproximate since in almost all cases only 1 of the objects is moving
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
                //Bottom side
                sideCollision = bottominsideother && !topinsideother && (leftinsideother || rightinsideother);
                if (sideCollision && bottomDistance == minDistance) {
                    side = 2;
                    normal = sf::Vector2<float>(0,  -1.f);
                }
                //Left side
                sideCollision = leftinsideother && !rightinsideother && (topinsideother || bottominsideother);
                if (sideCollision && leftDistance == minDistance) {
                    side = 3;
                    normal = sf::Vector2<float>(-1.f,  0.f);
                }

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

                //Take impact damage
                if (impactDamage > 0) {
                    int damage = int(velocity.y*impactDamage*0.005);
                    if (damage > 0)
                        TakeDamage(float(damage));
                }

                //Negate velocity / bounce
                float totalVelocity = -1*(normal*relativeVelocity)*(1+bounciness);

                //Apply velocity;
                bool normalNegative = normal.y < 0 || normal.x < 0;
                bool addNegative = totalVelocity < 0;
                if ((normalNegative && !addNegative) || (addNegative && !normalNegative))
                    velocity += normal * totalVelocity;

                if (normal != sf::Vector2<float>(0,0)) {
                    velocity += -relativeVelocity*friction*deltaTime.asSeconds();
                    test = true;
                }
            }
        }
    }
    colliding = test;



    //Scale the velocity to deltaTime to get consistent velocity across framerates
    position += velocity*deltaTime.asSeconds();

};

void Gameobject::SetVelocity(sf::Vector2<float> newVelocity) {
    velocity = newVelocity;
};

const sf::Vector2<float> Gameobject::GetVelocity() {
    return velocity;
};

void Gameobject::TakeDamage(float damage) {
};