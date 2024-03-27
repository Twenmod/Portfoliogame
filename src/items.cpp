#include "items.hpp"
#include "gameobject.hpp"
#include "worldgen.hpp"
#include "math.hpp"
#include "settings.hpp"
#include "globals.hpp"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstdlib>
#include "SFMLMath.hpp"

treasureItem::treasureItem() : Gameobject() {

}

treasureItem::treasureItem(int _value, float _startVelocity, Gameobject itemObject) : Gameobject(itemObject)
{
    value = _value;
    startVelocity = _startVelocity;
}
void treasureItem::PickUp() {
    //remove itself from the chunk
    destroyed = true;
}

void treasureItem::CalculatePhysics(std::vector<chunk*> chunkList) {

    velocity.y += gravity * deltaTime.asSeconds();

    //Collision detection

    bool test = false;

    //Get only chunks around current object
    std::vector<chunk*> localChunks;
    for (chunk* _chunk : chunkList) {

        sf::Vector2i mainchunkPos = currentChunk->chunkPosition;
        sf::Vector2i chunkPos = _chunk->chunkPosition;

        if (chunkPos.x >= mainchunkPos.x - 1 && chunkPos.x <= mainchunkPos.x + 1) {
            if (chunkPos.y >= mainchunkPos.y - 1 && chunkPos.y <= mainchunkPos.y + 1) {
                localChunks.push_back(_chunk);
            }
        }
    }

    for (chunk* _chunk : localChunks)
    {
        if (_chunk->collisionObjects.size() == 0) continue;

        for (Gameobject* otherobject : _chunk->collisionObjects)
        {
            if (!otherobject->hasCollision) {
                continue;
            }

            sf::Sprite* other = &otherobject->sprite;
            if (&sprite == other) continue;


            sf::Sprite _sprite = sprite;
            _sprite.setPosition(position);
            sf::FloatRect spriteRect = _sprite.getGlobalBounds();
            sf::FloatRect otherRect;


            //Use correct rect when colliding with player
            if (Player* playerobject = dynamic_cast<Player*>(otherobject))
            {
                otherRect = playerobject->playerRect;
                //make sure still collides otherwise continue to next object
                if (!spriteRect.intersects(otherRect)) {
                    continue;
                }
            }
            else {
                otherRect = other->getGlobalBounds();
            }

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

            bool topCollision = topinsideother && !bottominsideother && (leftinsideother || rightinsideother);
            bool rightCollision = rightinsideother && !leftinsideother && (topinsideother || bottominsideother);
            bool bottomCollision = bottominsideother && !topinsideother && (leftinsideother || rightinsideother);
            bool leftCollision = leftinsideother && !rightinsideother && (topinsideother || bottominsideother);

            if (!topCollision && !rightCollision && !bottomCollision && !leftCollision)
                continue; // Not colliding continue to next object



            //Find side by checking smallest distance between the sides                    
            float minDistance = INFINITY;
            int side = -1;

            float topDistance = math::difference(spriteRect.top, otherBottom); // Top
            if (topDistance < minDistance) {
                minDistance = topDistance;
            }
            float rightDistance = math::difference(spriteRight, otherRect.left); // Right
            if (rightDistance < minDistance) {
                minDistance = rightDistance;
            }
            float bottomDistance = math::difference(spriteBottom, otherRect.top); // Bottom
            if (bottomDistance < minDistance) {
                minDistance = bottomDistance;
            }
            float leftDistance = math::difference(spriteRect.left, otherRight); // Left
            if (leftDistance < minDistance) {
                minDistance = leftDistance;
            }

            //Not actually relative but good aproximate since in almost all cases only 1 of the objects is moving
            sf::Vector2<float> relativeVelocity = velocity;

            sf::Vector2<float> normal(0, 0);

            //Top side
            if (topCollision && topDistance == minDistance) {
                side = 0;
                normal = sf::Vector2<float>(0.f, 1.f);
            }

            //Right side
            if (rightCollision && rightDistance == minDistance) {
                side = 1;
                normal = sf::Vector2<float>(1.f, 0.f);
            }
            //Bottom side
            if (bottomCollision && bottomDistance == minDistance) {
                side = 2;
                normal = sf::Vector2<float>(0, -1.f);
            }
            //Left side
            if (leftCollision && leftDistance == minDistance) {
                side = 3;
                normal = sf::Vector2<float>(-1.f, 0.f);
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
                int damage = int(velocity.y * impactDamage * 0.005);
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
                sf::Vector2f drag = -relativeVelocity*friction*deltaTime.asSeconds();

                //Make sure drag only applies tangentally
                drag.x *= std::abs(normal.y); 
                drag.y *= std::abs(normal.x);
                        
                velocity += drag;
                test = true;
            }
        }
    }
    colliding = test;

    //Scale the velocity to deltaTime to get consistent velocity across framerates
    position += velocity*deltaTime.asSeconds();

};