#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include "gameobject.hpp"
#include "SFMLMath.hpp"

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

            /*Debug visualizer
            std::cout << "\n  T:"<< topinsideother << "\n"
            << leftinsideother << " o " << rightinsideother << "\n"
            << "  B:" << bottominsideother << "\n"; 
            */

            //Find side, set normal and set outside of other
            if (leftinsideother && !rightinsideother && topinsideother && bottominsideother) {normal = sf::Vector2<float>(-1.f,  0.f); position.x = otherRect.left+spriteRect.width; } /* left */
            if (topinsideother && !bottominsideother && (leftinsideother || rightinsideother)) {normal = sf::Vector2<float>( 0.f, 1.f); position.y = otherBottom+spriteRect.height; } /* bottom */
            if (bottominsideother && !topinsideother && (leftinsideother || rightinsideother)) {normal = sf::Vector2<float>( 0.f,  -1.f); position.y = otherRect.top-spriteRect.height; } /* top */
            if (rightinsideother && !leftinsideother && topinsideother && bottominsideother) {normal = sf::Vector2<float>( 1.f,  0.f); position.x = otherRight-spriteRect.width; } /* right */

            //Negate velocity / bounce
            float totalVelocity = -(1+bounciness)*(normal*relativeVelocity);
            //std::cout << "\nOldvelo:"<<velocity.y<<"\nAddVelo: " << totalVelocity;

            //Apply velocity;
            bool normalNegative = normal.y < 0 || normal.x < 0;
            bool addNegative = totalVelocity < 0;
            if ((normalNegative && !addNegative) || (addNegative && !normalNegative))
                velocity += normal * totalVelocity;

            //std::cout << "\n Normal: " << normal.y;
            if (normal != sf::Vector2<float>(0,0)) {
                velocity += -relativeVelocity*friction *std::clamp(deltaTime.asSeconds(),(float)0,(float)1);
                test = true;
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