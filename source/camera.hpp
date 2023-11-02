#ifndef CAMERA_H
#define CAMERA_H
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <list>
#include <vector>
#include "gameobject.hpp"

class Camera {
    public:
        //Constructor
        Camera(sf::Vector2<float> _position, sf::Vector2<float> _scale, sf::Vector2<float> _cullDistance);

        void render(sf::RenderWindow &window, std::vector<Gameobject> renderList);
    private:
        sf::Vector2<float> position;
        sf::Vector2<float> scale;
};

#endif