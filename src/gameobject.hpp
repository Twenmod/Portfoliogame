#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

//Forward declaration for chunks
class chunk;

class Gameobject {
        
    public:

        ///Transform

        sf::Vector2<float> position;
        float rotation;
        sf::Vector2<float> scale;

        ///Sprite

        bool hasSprite;
        std::vector<sf::Texture*> textures;
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
        
        bool destroyed = false;

        chunk* currentChunk;

        Gameobject(); // Default constructor
        Gameobject(sf::Vector2<float> _position, float _rotation,sf::Vector2<float> _size, bool _hasSprite, std::vector<sf::Texture*> _texture, bool _isStatic, bool  _hasCollision, float _gravity, float _friction, float _bounciness, sf::Vector2<float> _startVelocity);
        void resetTexture(); //Resets the texture to a random one in its textures
        void updateCurrentChunk(std::vector<std::vector<chunk*>> chunkList);
        void SetScale(sf::Vector2<float> setscale);
        void SetVelocity(sf::Vector2<float> newVelocity);
        sf::Vector2<float> GetVelocity();
        void OnEvent();
        virtual void OnLoop(std::vector<chunk*> chunkList);
        virtual void OnRender();
        virtual void CalculatePhysics(std::vector<chunk*> chunkList);
        virtual void TakeDamage(float damage);

    private:
};

#endif