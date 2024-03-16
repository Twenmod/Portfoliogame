#ifndef EXPLOSIVE_H
#define EXPLOSIVE_H

#include "gameobject.hpp"
#include <SFML/Graphics/Texture.hpp>

class explosiveObject : public Gameobject {
    public:
        explosiveObject(); // Default constructor
        explosiveObject(float health, int explosionRadius, float damage, float animationInterval, int animationFrames, sf::Texture* explosionTexture, Gameobject gameObject);
        float health;
        int explosionRadius;
        int animationInterval;
        int animationFrames;
        float damage;
        sf::Texture* explosionTexture;
        int frame = 0;
        float timer = 0;
        bool exploding = false;
        bool exploded = false;
        void TakeDamage(float damage);
        void Explode();
        void OnLoop(std::vector<chunk*> chunkList);
        void OnRender();
};


#endif