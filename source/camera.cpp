#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include "camera.hpp"
#include "gameobject.hpp"

Camera::Camera(sf::Vector2<float> _position, sf::Vector2<float> _scale, sf::Vector2<float> _cullDistance) {
    position = _position;
    scale = _scale;
    cullDistance = _cullDistance;
    followTarget = nullptr;
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

void Camera::OnLoop(sf::Time deltaTime) {
    //Lerp* to target
    if (followTarget != nullptr) {
        std::cout << "\nTargetPos: " << followTarget->position.x;
        // X
        position.x = position.x-deltaTime.asSeconds()*lerpSpeed*(position.x - followTarget->position.x);
        // Y
        position.y = position.y-deltaTime.asSeconds()*lerpSpeed*(position.y - followTarget->position.y);
    }
}

void Camera::SetObjectToFollow(Gameobject* target, float lerpSpeed) {
    Camera::followTarget = target;
};
