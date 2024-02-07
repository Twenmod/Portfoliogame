#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "camera.hpp"
#include "gameobject.hpp"
#include "settings.hpp"
#include "worldgen.hpp"

uiElement::uiElement(sf::Text _text, sf::Font _font) {
    text = _text;
    font = _font;
    text.setFont(font);
};

Camera::Camera(sf::Vector2<float> _position, sf::Vector2<float> _scale,sf::Vector2<unsigned int> _resolution) {
    position = _position;
    scale = _scale;
    resolution = _resolution;
    followTarget = nullptr;

};

void Camera::Render(sf::RenderWindow &window,Gameobject* player, std::vector<chunk*> chunkList, std::vector<uiElement*> uiElements) {
    //Render objects
    for (chunk* chunk : chunkList) {
        for(Gameobject* obj : chunk->objects) {
            obj->OnRender();

            //Only render if object contains a sprite
            if (obj->hasSprite) {
                sf::Vector2<float> spritePos = obj->sprite.getPosition();

                sf::Vector2<float> cameraWorldPosition = position;
                cameraWorldPosition.x += (float)resolution.x/2;
                cameraWorldPosition.y += (float)resolution.y/2;

                bool outSideCullDistance = 
                    (spritePos.x > cameraWorldPosition.x + globalsettings.cullSize.x) 
                    || 
                    (spritePos.x < cameraWorldPosition.x - globalsettings.cullSize.x)
                    ||
                    (spritePos.y > cameraWorldPosition.y + globalsettings.cullSize.y)
                    ||
                    (spritePos.y < cameraWorldPosition.y - globalsettings.cullSize.y);
                if (!outSideCullDistance) {
                    //Copy world space sprite to a screen space sprite
                    sf::Sprite spriteToDraw = obj->sprite;
                    spriteToDraw.setPosition(spriteToDraw.getPosition() - position); 
                

                    window.draw(spriteToDraw);

                    if (tile* _tile = dynamic_cast<tile*>(obj)) {
                        if (_tile->health != _tile->maxHealth) {
                            //Tile has damage so draw damage sprite
                            int texturesize = tileBreakTexture->getSize().y;
                            int Slices = tileBreakTexture->getSize().x/texturesize;
                            int slice = std::round((_tile->health/_tile->maxHealth)*Slices);


                            sf::Sprite damageOverlay;
                            damageOverlay.setTexture(*tileBreakTexture);
                            damageOverlay.setTextureRect(sf::IntRect(tileBreakTexture->getSize().x-slice*texturesize,0,texturesize,texturesize));
                            damageOverlay.setPosition(spriteToDraw.getPosition());
                            damageOverlay.setScale(spriteToDraw.getScale());
                            window.draw(damageOverlay);
                        }

                    }

                }
            }
        }
    }

    //Render player
    player->OnRender();
    sf::Sprite playerSprite = player->sprite;
    playerSprite.setPosition(playerSprite.getPosition() - position); 
    window.draw(playerSprite);

    //Render UI
    for(uiElement* text : uiElements) {

        // Check if text.text is not null before drawing
        if (text->text.getFont() != nullptr) {
            window.draw(text->text);
        } else {
            std::cout << "Error: Font pointer is null for textElement." << std::endl;
        }    
    }
};

void Camera::OnLoop(sf::RenderWindow &window) {
    //Lerp to target
    if (followTarget != nullptr) {
        sf::FloatRect targetRect = followTarget->sprite.getGlobalBounds();
        
        // X
        position.x = position.x+std::clamp(deltaTime.asSeconds()*lerpSpeed*((followTarget->position.x - float(globalsettings.windowSize.x)/2+(targetRect.width/4)) - position.x),-lerpSpeed*10,lerpSpeed*10);
        // Y
        position.y = position.y+std::clamp(deltaTime.asSeconds()*lerpSpeed*((followTarget->position.y - float(resolution.y)/2+(targetRect.height/4)) - position.y),-lerpSpeed*10,lerpSpeed*10);
    }
}

void Camera::SetObjectToFollow(Gameobject* target, float lerpSpeed) {
    Camera::followTarget = target;
    Camera::lerpSpeed = lerpSpeed;
    position = target->position;

};
