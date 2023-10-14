#include <iostream>
#include "gameobject.hpp"
#include "gmath.hpp"

sf::Texture emptyimage();

//Constructor
Gameobject::Gameobject(sf::Vector2<float> _position = sf::Vector2<float>(0,0), float _rotation = 0,sf::Vector2<float> _scale =sf::Vector2<float>(1,1), bool _hasSprite = false, sf::Texture* _texture = nullptr, bool _isStatic = false, bool _hasCollision = true, float _gravity = 10, float _drag = 0) {

    position = _position;
    rotation = _rotation;
    scale = _scale;

    ///Sprite

    hasSprite = _hasSprite;
    if (hasSprite) {

        sprite.setTexture(*_texture);
        SetScale(_scale);
    }


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
void Gameobject::OnLoop(sf::Time deltaTime) {
    if (!isStatic) {
        CalculatePhysics(deltaTime);
    }
};

//Called just before render
void Gameobject::OnRender() {
    if(hasSprite) {
        sprite.setPosition(position);
        sprite.setRotation(rotation);
    }
};

void Gameobject::CalculatePhysics(sf::Time deltaTime) {

    velocity.y += gravity;

    //Scale the velocity to deltaTime to get consistent velocity across framerates
    sf::Vector2<float> scaledVelocity(velocity.x*deltaTime.asSeconds(),velocity.y*deltaTime.asSeconds());
    position += scaledVelocity;

    //Collision detection



};