#ifndef CAMERA_H
#define CAMERA_H
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <list>
#include <vector>
#include "gameobject.hpp"

class Camera {
    public:
        //Constructor
        Camera(sf::Vector2<float> _position = sf::Vector2<float>(0,0), sf::Vector2<float> _scale = sf::Vector2<float>(1,1), sf::Vector2<float> _cullDistance = sf::Vector2<float>(500,500));

        void Render(sf::RenderWindow &window, std::vector<Gameobject> renderList);

        void OnLoop(sf::Time deltaTime);

        void SetObjectToFollow(Gameobject* target, float lerpSpeed);
    private:
        sf::Vector2<float> position;
        sf::Vector2<float> scale;
        sf::Vector2<float> cullDistance;
        Gameobject* followTarget;
        float lerpSpeed = 2;
};

#endif