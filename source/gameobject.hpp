#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <iostream>
#include <SFML/Graphics.hpp>

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
        float bounciness = 0.1;
        float friction = 30;

        Gameobject(sf::Vector2<float> _position, float _rotation,sf::Vector2<float> _scale, bool _hasSprite, sf::Texture* _texture, bool _isStatic, bool  _hasCollision, float _gravity, float _drag);
        void SetScale(sf::Vector2<float> setscale);
        void OnEvent();
        void OnLoop(sf::Time deltaTime, std::vector<sf::Sprite*> collisionList);
        void OnRender();
    private:
        sf::Vector2<float> velocity;
        bool colliding;
        void CalculatePhysics(sf::Time deltaTime, std::vector<sf::Sprite*> collisionList);
};

#endif