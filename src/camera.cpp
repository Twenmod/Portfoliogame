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

uiElement::uiElement(sf::Text _text, sf::Font _font, bool startEnabled) {
    text = _text;
    font = _font;
    text.setFont(font);
    enabled = startEnabled;
};

uiElement generateUIElement(sf::Font font, int fontSize, sf::Color color, sf::Text::Style style, sf::Vector2<float> position, std::string text, bool startEnabled) {
    //Create Text and set values
    sf::Text textElement;
    textElement.setCharacterSize(fontSize);
    textElement.setFillColor(color); //rgb(116 12 12)
    textElement.setStyle(style);
    textElement.setPosition(position);
    textElement.setString(text);

    //Create uiElement and return it
    uiElement element(textElement, font, startEnabled);

    return element;
};

Camera::Camera() {

};

Camera::Camera(sf::Texture* damageOverlay, sf::Vector2<float> _position, sf::Vector2<float> _scale,sf::Vector2<unsigned int> _resolution) {
    position = _position;
    scale = _scale;
    resolution = _resolution;
    followTarget = nullptr;
    damageOverlayTexture = damageOverlay;
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

    ////Render Damage Overlay

    //Get player object
    Player* playerobj = dynamic_cast<Player*>(player);
    //Get overlay limit (basically how visible it is on screen)
    overlayLimit = ((playerobj->health-1) / playerobj->maxHealth) * maxOverlaySize;

    //Slowly fade out overlay by scaling it
    if (currentOverlaySize < overlayLimit) {
    currentOverlaySize += deltaTime.asSeconds()*overlaySpeed;
    } else {
        currentOverlaySize = overlayLimit;
    }
    if (currentOverlaySize < maxOverlaySize) {

        //Draw overlay
        sf ::Sprite overlay = sf::Sprite();
        overlay.setTexture(*damageOverlayTexture);
        float minimumScale = (float)resolution.x/(float)damageOverlayTexture->getSize().x;
        overlay.setScale(currentOverlaySize+minimumScale,currentOverlaySize+minimumScale);
        //Set center of sprite to center of screen
        overlay.setPosition(sf::Vector2<float>((float)resolution.x/2,(float)resolution.y/2)-sf::Vector2<float>(overlay.getGlobalBounds().width/2,overlay.getGlobalBounds().height/2));
        
        window.draw(overlay);
    }

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
