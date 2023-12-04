#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <SFML/System/Vector2.hpp>
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
        float bounciness = 0.2;
        float friction = 2;

        sf::Vector2<float> velocity;
        bool colliding;

        Gameobject(std::vector<sf::Sprite*> &collisionList, sf::Vector2<float> _position, float _rotation,sf::Vector2<float> _scale, bool _hasSprite, sf::Texture* _texture, bool _isStatic, bool  _hasCollision, float _gravity, float _friction, float _bounciness, sf::Vector2<float> _startVelocity);
        void SetScale(sf::Vector2<float> setscale);
        void SetVelocity(sf::Vector2<float> newVelocity);
        sf::Vector2<float> GetVelocity();
        void OnEvent();
        virtual void OnLoop(sf::Time deltaTime, std::vector<sf::Sprite*> collisionList);
        void OnRender();
    private:
        virtual void CalculatePhysics(sf::Time deltaTime, std::vector<sf::Sprite*> collisionList);
};

#endif