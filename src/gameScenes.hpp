#ifndef GAMESCENES_H
#define GAMESCENES_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include "camera.hpp"
#include "worldgen.hpp"


class mainMenu {

    public:

        sf::RenderWindow* gameWindow;
        std::vector<uiElement*> uiElements;

        bool startGameTrigger = false;

        //Constructer/Init
        mainMenu(sf::RenderWindow &window) {
            gameWindow = &window;
        }

        void OnEvents();
        void OnLoop(sf::RenderWindow &window);
        void OnRender(sf::RenderWindow &window);
};

//The game itself
class mainLevel {
    public:
        Camera mainCamera;

        std::vector<uiElement*> uiElements;
        std::vector<uiSprite*> uiSprites;


        std::vector<std::vector<chunk*>> chunkList;
        std::vector<chunk*> activeChunkList;
        
        sf::Clock gameClock; 
        sf::Clock renderClock;

        sf::RenderWindow* gameWindow;

        //Chunks
        sf::Vector2<int> playerChunkPosition;

        Gameobject* player;

        //Fps calculator
        int fpsI = 0;
        float fpsArray[60];

        //Constructer/Init
        mainLevel(sf::RenderWindow &window, sf::Texture* damageOverlay) {
            gameWindow = &window;
            mainCamera = generateCamera(damageOverlay, sf::Vector2<float>(0,0), sf::Vector2<float>(1,1));
        }

        Camera generateCamera(sf::Texture* damageOverlay, sf::Vector2<float> startPosition, sf::Vector2<float> scale) {
            return Camera(damageOverlay, startPosition,scale, gameWindow->getSize());
        }

        void OnEvents();
        void OnLoop(sf::RenderWindow &window);
        void OnRender(sf::RenderWindow &window);
};


#endif