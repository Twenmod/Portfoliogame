#include "gameScenes.hpp"
#include "settings.hpp"


#pragma region mainMenu


void mainMenu::OnEvents() {
    sf::Vector2i localMousePosition = sf::Mouse::getPosition(*gameWindow);
    bool isPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    //Check collision with uiElements
    for (uiElement* textElement : uiElements) {
        if (!textElement->hasInput) continue;
        sf::Rect textRect = textElement->text.getGlobalBounds();
        bool insideHorizontal = (localMousePosition.x > textRect.left) && (localMousePosition.x < textRect.left + textRect.width);
        bool insideVertical = (localMousePosition.y > textRect.top) && (localMousePosition.y < textRect.top + textRect.height);
        if (insideHorizontal && insideVertical) {
            textElement->hovering = true;
            if (isPressed) textElement->holding = true;
            else {
                if (textElement->holding) {
                    //Click
                    textElement->onClickFunction();
                    
                }
                textElement->holding = false;
            }
        }
        else {
            textElement->holding = false;
            textElement->hovering = false;
        }
    }
};
void mainMenu::OnLoop(sf::RenderWindow &window) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        startGameTrigger = true;
    }
};
void mainMenu::OnRender(sf::RenderWindow &window) {
    
    window.display();
    window.clear(globalsettings.backgroundColor);

    //Render UI
    ///Render ui texts
    for(uiElement* text : uiElements) {
        if (text != nullptr && text->enabled) {
            // Check if text.text is not null before drawing
            if (text->text.getFont() != nullptr) {
                if (text->hovering) text->text.setFillColor(text->hoverColor);
                else text->text.setFillColor(text->startColor);
                window.draw(text->text);
            } else {
                std::cout << "Error: Font pointer is null for textElement." << std::endl;
            }    
        }
    }
};

#pragma endregion

#pragma region mainLevel

void mainLevel::OnEvents() {
    for (chunk* _chunk : activeChunkList) {
        _chunk->OnEvents();
    }
};

void mainLevel::OnLoop(sf::RenderWindow &window) {
    //Get time elapsed since last frame
    deltaTime = gameClock.restart();
    deltaTime = sf::Time(sf::seconds(std::clamp(deltaTime.asSeconds(),0.0f,globalsettings.maxDeltaTime)));
    


    //Get active chunks
    sf::Vector2<int> _playerchunkPos;
    _playerchunkPos.x = (int)(player->position.x/globalsettings.tileSize/globalsettings.chunkSize);
    _playerchunkPos.y = (int)(player->position.y/globalsettings.tileSize/globalsettings.chunkSize);
    if (_playerchunkPos != playerChunkPosition) {
        //Player moved to new chunk so update active chunks
        playerChunkPosition = _playerchunkPos;
        activeChunkList.clear();
        for (int x = -globalsettings.chunkLoadDistance; x < globalsettings.chunkLoadDistance+1; x++) {
            for (int y = -globalsettings.chunkLoadDistance; y < globalsettings.chunkLoadDistance+1; y++) {
                int chunkposx = _playerchunkPos.x + x;
                int chunkposy = _playerchunkPos.y + y;
                //Check if not out of bounds
                if (!((chunkposx < 0 || chunkposx > globalsettings.worldSize.x-1) || (chunkposy < 0 || chunkposy > globalsettings.worldSize.y-1))) {
                    activeChunkList.push_back(chunkList[chunkposx][chunkposy]);
                }
            }
        }
    }

    //Calculate Objects
    for (chunk* _chunk : activeChunkList) {
        _chunk->OnLoop(activeChunkList, chunkList);
    }

    //Calculate Player
    player->OnLoop(activeChunkList);



    //Set UI
    fpsVec.insert(fpsVec.begin(), 1 / deltaTime.asSeconds());
    int fpsMaxVectorSize = 5 / deltaTime.asSeconds(); // Takes average fps of last 5 seconds
    if (fpsVec.size() > fpsMaxVectorSize)
        fpsVec.pop_back();
    float avarageFps = 0;
    for (int i = 0; i < fpsVec.size(); i++) {
        avarageFps += fpsVec[i];
    }
    avarageFps /= fpsVec.size();
    uiElements[0]->text.setString("FPS: " + std::to_string((int)avarageFps));

    if (Player *_player = dynamic_cast<Player*>(player)) {
        uiElements[1]->text.setString("Health: " + std::to_string((int)_player->health) + "/" + std::to_string((int)_player->maxHealth));
        uiElements[2]->text.setString("Gold: " + std::to_string(_player->gold));
    }

};

void mainLevel::OnRender(sf::RenderWindow& window) {
    renderDeltaTime = renderClock.restart();
    renderDeltaTime = sf::Time(sf::seconds(std::clamp(renderDeltaTime.asSeconds(), 0.0f, globalsettings.maxDeltaTime)));

    mainCamera.Render(window, player, activeChunkList, uiElements, uiSprites);
}

#pragma endregion