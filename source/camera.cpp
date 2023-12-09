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

void Camera::Render(sf::RenderWindow &window, std::vector<Gameobject*> renderList) {
    for(Gameobject* obj : renderList) {
        obj->OnRender();

        //Only render if object contains a sprite
        if (obj->hasSprite) {
            //Copy world space sprite to a screen space sprite
            sf::Sprite spriteToDraw = obj->sprite;
            spriteToDraw.setPosition(spriteToDraw.getPosition() + position); 
            
            //std::cout << "Drawing sprite at: " << spriteToDraw.getPosition().x << ", " << spriteToDraw.getPosition().y << std::endl;

            window.draw(spriteToDraw);
        }
    }
};

void Camera::OnLoop(sf::Time deltaTime, sf::RenderWindow &window) {
    //Lerp to target
    if (followTarget != nullptr) {
        sf::FloatRect targetRect = followTarget->sprite.getGlobalBounds();


        //std::cout << "\nTargetPos: " << followTarget->position.x << " CurrentPos: "<<position.x<< " Window: "<< window.getSize().x;
        
        // X
        position.x = position.x+std::clamp(deltaTime.asSeconds()*lerpSpeed*((-followTarget->position.x+ float(sf::VideoMode::getDesktopMode().width)/2-(targetRect.width/2)) - position.x),-lerpSpeed*10,lerpSpeed*10);
        // Y
        position.y = position.y+std::clamp(deltaTime.asSeconds()*lerpSpeed*((-followTarget->position.y+float(sf::VideoMode::getDesktopMode().height)/2-(targetRect.height/2)) - position.y),-lerpSpeed*10,lerpSpeed*10);
    }
}

void Camera::SetObjectToFollow(Gameobject* target, float lerpSpeed) {
    Camera::followTarget = target;
    Camera::lerpSpeed = lerpSpeed;

};
