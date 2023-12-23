#ifndef CAMERA_H
#define CAMERA_H
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <list>
#include <vector>
#include "gameobject.hpp"


class uiElement {
    public:
        sf::Text text;
        sf::Font font;
        uiElement(sf::Text _text, sf::Font _font);
};

class Camera {
    public:
        //Constructor
        Camera(sf::Vector2<float> _position = sf::Vector2<float>(0,0), sf::Vector2<float> _scale = sf::Vector2<float>(1,1),sf::Vector2<unsigned int> _resolution = sf::Vector2<unsigned int>(0,0));

        void Render(sf::RenderWindow &window, std::vector<Gameobject*> renderList, std::vector<uiElement*> uiElements);

        void OnLoop(sf::RenderWindow &window);

        void SetObjectToFollow(Gameobject* target, float lerpSpeed);

    private:
        sf::Vector2<unsigned int> resolution;
        sf::Vector2<float> position;
        sf::Vector2<float> scale;
        sf::Vector2<float> cullDistance;
        Gameobject* followTarget;
        float lerpSpeed = 1;
};

#endif