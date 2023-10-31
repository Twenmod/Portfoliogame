#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <vector>
#include "gameobject.hpp"
#include "SFMLMath.hpp"

sf::Texture emptyimage();

//Constructor
Gameobject::Gameobject(sf::Vector2<float> _position, float _rotation = 0,sf::Vector2<float> _scale =sf::Vector2<float>(1,1), bool _hasSprite = false, sf::Texture* _texture = nullptr, bool _isStatic = false, bool _hasCollision = true, float _gravity = 10, float _drag = 0) {

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

    velocity = sf::Vector2<float>(5,0);

    //Physics
    isStatic = _isStatic;
    hasCollision = _hasCollision;
    gravity = _gravity;
    drag = _drag;



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

    velocity.y += gravity;

    //Collision detection

    bool test = false;

    for (sf::Sprite* other : collisionList) {
        if (&sprite != other) {
            sf::FloatRect spriteRect = sprite.getGlobalBounds();
            sf::FloatRect otherRect = other->getGlobalBounds();
            //Colliding!
            //Test all sides

            sf::Vector2<float> relativeVelocity = velocity;

            sf::Vector2<float> dirfrac;

            sf::Vector2<float> normal;

            float spriteBottom = spriteRect.top - spriteRect.height;
            float otherBottom = otherRect.top - otherRect.height;
            float spriteRight = spriteRect.left + spriteRect.width;
            float otherRight = otherRect.left + otherRect.width;

            bool bottominsideother = spriteRect.top >= otherBottom && spriteRect.top <= otherRect.top;
            bool topinsideother = spriteBottom <= otherRect.top && spriteBottom >= otherBottom;
            bool leftinsideother = spriteRect.left >= otherRect.left && spriteRect.left <= otherRight;
            bool rightinsideother = spriteRight >= otherRect.left && spriteRight <= otherRight;


            if (leftinsideother && !rightinsideother && topinsideother && bottominsideother) normal = sf::Vector2<float>(-1.f,  0.f); /* left */
            if (topinsideother && !bottominsideother && leftinsideother || rightinsideother) normal = sf::Vector2<float>( 0.f, -1.f); /* bottom */
            if (bottominsideother && !topinsideother && leftinsideother || rightinsideother) normal = sf::Vector2<float>( 0.f,  1.f); /* top */
            if (rightinsideother && !leftinsideother && topinsideother && bottominsideother) normal = sf::Vector2<float>( 1.f,  0.f); /* right */

            float totalVelocity = -(1+bounciness)*(normal*relativeVelocity);

            velocity += normal * totalVelocity;
            }

            test = true;

        }
    colliding = test;

    //Scale the velocity to deltaTime to get consistent velocity across framerates
    position += velocity*deltaTime.asSeconds();

};

