#include "explosive.hpp"
#include "SFMLMath.hpp"
#include "gameobject.hpp"
#include "settings.hpp"
#include "worldgen.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>

explosiveObject::explosiveObject(float _health, int _explosionRadius, float _damage, float _animationInterval, int _animationFrames, sf::Texture* _explosionTexture, Gameobject _gameObject) : Gameobject(_gameObject) {
	health = _health;
	explosionRadius = _explosionRadius;
	animationInterval = _animationInterval;
	animationFrames = _animationFrames;
    explosionTexture = _explosionTexture;
    _damage = damage;
};

void explosiveObject::TakeDamage(float damage) {
    health -= damage;
    if (health <= 0) {
        Explode();
    }
}

void explosiveObject::OnLoop(std::vector<chunk*> chunkList) {
    Gameobject::OnLoop(chunkList);
    if (exploding && !exploded) {
        exploded = true;

        //Deal damage to all objects in explosion radius
        for (chunk* _chunk : chunkList) {
            for (Gameobject* object : _chunk->objects) {
                //Get distance to object
                sf::Vector2f offset = object->position - position;
                float distance = sf::sqr(offset.x * offset.x + offset.y * offset.y);
                if (distance < explosionRadius) {
                    //Deal damage to object
                    //Damage is reduced exponentally by distance
                    float dmg = damage * std::pow(1 - distance / explosionRadius, 2);
                    object->TakeDamage(dmg);
                }
            }
        }
    }
}

void explosiveObject::OnRender() {
    if (exploding) {
        //Draw explosion animation
        timer -= deltaTime.asSeconds();
        if (timer <= 0) {
            timer = animationInterval;
            frame++;
            sprite.setTextureRect(sf::IntRect(0,0, explosionTexture->getSize().y * frame, explosionTexture->getSize().y));
        }

    }

    Gameobject::OnRender();
};

void explosiveObject::Explode() {
    //Queue explosion because we do not have access to the active chunklist via takedamage
    exploding = true;

    //Disable all dynamics
    isStatic = true;
    hasCollision = false;

    //Set up explosion animation
    timer = animationInterval;
    sprite.setTexture(*explosionTexture);
    //Use y as x size because explosion texture is square and in a sheet
    sprite.setTextureRect(sf::IntRect(0, 0, explosionTexture->getSize().y, explosionTexture->getSize().y));
};