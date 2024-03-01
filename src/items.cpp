#include "items.hpp"
#include "gameobject.hpp"
#include "worldgen.hpp"
#include "math.hpp"
#include "settings.hpp"
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

void treasureItem::CalculatePhysics(std::vector<chunk*> chunkList) { //TODO: Fix collision acting weard on windows specifically

    velocity.y += gravity * deltaTime.asSeconds();

    //Collision detection

    bool test = false;

    for (chunk* _chunk : chunkList) 
    {
        if (_chunk->collisionObjects.size() == 0) continue;
        for (Gameobject* otherobject : _chunk->collisionObjects) 
        {
            sf::Sprite* other = &otherobject->sprite;
            if (&sprite != other) 
            {
                if (otherobject->objectName == "Player") continue;

                if (sprite.getGlobalBounds().intersects(other->getGlobalBounds())) 
                {
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


                    //Find side by checking smallest distance between the sides
                    

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
        }
    }
    colliding = test;

    //Scale the velocity to deltaTime to get consistent velocity across framerates
    position += velocity*deltaTime.asSeconds();

};