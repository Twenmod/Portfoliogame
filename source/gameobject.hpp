#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include "gmath.hpp"

class Gameobject {
        
    public:

        ///Transform

       sf::Vector2<float> position;
        float rotation;
       sf::Vector2<float> scale;

        ///Sprite

        bool hasSprite;
        sf::Texture texture;
        sf::Sprite sprite;

        ///Physics

        bool isStatic;
        bool hasCollision;
        float gravity;
        float drag;

        Gameobject(sf::Vector2<float> _position, float _rotation,sf::Vector2<float> _scale, bool _hasSprite, sf::Texture* _texture, bool  _hasCollision, bool _isStatic, float _gravity, float _drag);
        void SetScale(sf::Vector2<float> setscale);
        void OnEvent();
        void OnLoop(sf::Time deltaTime);
        void OnRender();
    private:
        sf::Vector2<float> velocity;
        void CalculatePhysics(sf::Time deltaTime);
};

#endif