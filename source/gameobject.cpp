#include <iostream>
#include "gameobject.hpp"
#include "gmath.hpp"

//Constructor
Gameobject::Gameobject(sf::Vector2<float> _position = sf::Vector2<float>(0,0), float _rotation = 0,sf::Vector2<float> _scale =sf::Vector2<float>(1,1), bool _hasSprite = false, std::string textureLocation = "", bool _isStatic = false, float _gravity = 10, float _drag = 0) {

    position = _position;
    rotation = _rotation;
    scale = _scale;

    ///Sprite

    hasSprite = _hasSprite;
    if (hasSprite) {

        //Load the texture
        if (!texture.loadFromFile(textureLocation))
        {
            // failed to load texture
            std::cout<<"Warning: Gameobject tried to load sprite but failed";
        }

        sprite.setTexture(texture);
        SetScale(_scale);
    }


    //Physics
    isStatic = _isStatic;
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
void Gameobject::OnLoop() {
    if (!isStatic) {
        CalculatePhysics();
    }
};

//Called just before render
void Gameobject::OnRender() {
    if(hasSprite) {
        sprite.setPosition(position);
        sprite.setRotation(rotation);
    }
};

void Gameobject::CalculatePhysics() {
    std::cout<<velocity.y<<"\n";
    position += velocity;

    velocity.y = gravity;
};