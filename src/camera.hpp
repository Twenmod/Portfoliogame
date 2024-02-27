#ifndef CAMERA_H
#define CAMERA_H
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include "gameobject.hpp"
#include "globals.hpp"


class uiElement {
    public:
        sf::Text text;
        bool enabled;
        uiElement(sf::Text _text, const sf::Font& _font, bool startEnabled = true);
};


uiElement generateUIElement(const sf::Font& font, int fontSize = 100, sf::Color color = sf::Color(116, 12, 12), sf::Text::Style style = sf::Text::Bold, sf::Vector2<float> position = sf::Vector2<float>(0,0), std::string text = "Spelunker", bool startEnabled = true);

class uiSprite {
    public:
        sf::Sprite sprite;
        bool enabled;
        uiSprite(sf::Sprite _sprite, bool startEnabled = true);
};

uiSprite generateUISprite(sf::Texture* texture, sf::Vector2<float> position = sf::Vector2<float>(0,0), sf::Vector2<float> size = sf::Vector2<float>(64,64), bool startEnabled = true);

class Camera {
    public:
        //Constructor
        Camera();
        Camera(sf::Texture* damageOverlay, sf::Vector2<float> _position = sf::Vector2<float>(0,0), sf::Vector2<float> _scale = sf::Vector2<float>(1,1),sf::Vector2<unsigned int> _resolution = sf::Vector2<unsigned int>(0,0));

        void Render(sf::RenderWindow &window, Gameobject* player, std::vector<chunk*> chunkList, std::vector<uiElement*> uiElements, std::vector<uiSprite*> uiSprites);

        void OnLoop(sf::RenderWindow &window);

        void SetObjectToFollow(Gameobject* target, float lerpSpeed);
        sf::Texture* tileBreakTexture;


        float currentOverlaySize;

    private:
        sf::Vector2<unsigned int> resolution;
        sf::Vector2<float> position;
        sf::Vector2<float> scale;
        sf::Vector2<float> cullDistance;
        sf::Texture* damageOverlayTexture;
        Gameobject* followTarget;
        float lerpSpeed = 1;

        //Damage Overlay
        float overlayLimit;
        float overlaySpeed = 1;
        float maxOverlaySize = 0.4f;

};

#endif