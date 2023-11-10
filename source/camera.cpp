#include <SFML/Graphics.hpp>
#include "camera.hpp"
#include "gameobject.hpp"

Camera::Camera(sf::Vector2<float> _position, sf::Vector2<float> _scale, sf::Vector2<float> _cullDistance) {
    position = _position;
    scale = _scale;
    cullDistance = _cullDistance;
};

void Camera::Render(sf::RenderWindow &window, std::vector<Gameobject> renderList) {
    for(Gameobject obj : renderList) {
        obj.OnRender();

        //Only render if object contains a sprite
        if (obj.hasSprite) {
            //Copy world space sprite to a screen space sprite
            sf::Sprite spriteToDraw = obj.sprite;
            spriteToDraw.setPosition(spriteToDraw.getPosition()+position); 
            window.draw(spriteToDraw);
        } 
    }
};
