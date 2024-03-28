#include "gameScenes.hpp"
#include "settings.hpp"


#pragma region mainMenu


void mainMenu::OnEvents() {

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
    
    //Calculate camera
    mainCamera.OnLoop(window);

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
    fpsArray[fpsI] = 1/deltaTime.asSeconds();
    fpsI++;
    int fpsArraySize = int(std::size(fpsArray));
    if (fpsI >= fpsArraySize)
        fpsI = 0;
    float avarageFps = 0;
    for (int i = 0; i < fpsArraySize; i++) {
        avarageFps += fpsArray[i];
    }
    avarageFps /= fpsArraySize;
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