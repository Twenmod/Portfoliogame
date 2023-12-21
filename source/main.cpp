#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "gameobject.hpp"
#include "camera.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "settings.hpp"
#include "worldgen.hpp"
#include <SFML/Window/WindowStyle.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <utility>
#include <vector>

sf::Vector2<int> resolution(200,200);

Settings globalsettings = Settings();
sf::Time deltaTime;

//The game itself
class app {
    public:
        Camera mainCamera;


        //Lists of the objects split for optimisation
        std::vector<Gameobject*> objectList;
        std::vector<sf::Sprite*> collisionList;
        std::vector<sf::Text*> uiElements;
        
        sf::Clock gameClock; 

        sf::RenderWindow* gameWindow;

        //Fps calculator
        int fpsI = 0;
        float fpsArray[10];

        //Constructer/Init
        app(sf::RenderWindow &window) {
            gameWindow = &window;
            mainCamera = generateCamera(sf::Vector2<float>(0,0), sf::Vector2<float>(1,1));
        }

        Camera generateCamera(sf::Vector2<float> startPosition, sf::Vector2<float> scale) {
            return Camera(startPosition,scale,gameWindow->getSize());
        }

        void OnEvents() {
            for (Gameobject* obj : objectList) {
                obj->OnEvent();
            }
        };
        void OnLoop(sf::RenderWindow &window) {
            //Get time elapsed since last frame
            deltaTime = gameClock.restart();
            deltaTime = sf::Time(sf::seconds(std::clamp(deltaTime.asSeconds(),0.0f,globalsettings.maxDeltaTime)));
            
            //Calculate camera
            mainCamera.OnLoop(window);
            //Calculate objects
            for (Gameobject* obj : objectList) {
                obj->OnLoop(collisionList);
            }

            //Set UI
            fpsArray[fpsI] = 1/deltaTime.asSeconds();
            fpsI++;
            int fpsArraySize = std::size(fpsArray);
            if (fpsI >= fpsArraySize)
                fpsI = 0;
            float avarageFps = 0;
            for (int i = 0; i < fpsArraySize; i++) {
                avarageFps += fpsArray[i];
            }
            avarageFps /= fpsArraySize;
            uiElements[0]->setString("FPS: " + std::to_string((int)avarageFps));

        };
        void OnRender(sf::RenderWindow &window) {
            mainCamera.Render(window,objectList, uiElements);
        };
};


sf::Texture addTexture(std::string file) {
    sf::Texture text;
    if (!text.loadFromFile(file)) {
        std::cout << "Failed to load texture at: " << file;
    }
    return text;
}

int main()
{


    //Initialize window
    sf::RenderWindow window(sf::VideoMode((int)globalsettings.windowSize.x,(int)globalsettings.windowSize.y), "Minergame", sf::Style::Close);

    window.setVerticalSyncEnabled(true);
    window.setActive(true);
    window.setFramerateLimit(globalsettings.frameRateLimit);

    //Map of the games textures

    std::map<std::string, sf::Texture> texturemap = {
        {"Square", addTexture("Sprites/Square.jpg")},
        {"Dirt", addTexture("Sprites/Tiles/Dirt.png")},
        {"Stone",addTexture("Sprites/Tiles/Stone.png")},
        {"Gold",addTexture("Sprites/Tiles/Gold.png")},
        {"Noomba",addTexture("Sprites/noomba.png")},
        {"Bedrock",addTexture("Sprites/Tiles/Bedrock.png")},
    };

    //Load game
    app game(window);

    //Load level
    Player player = Player(globalsettings.playerMoveSpeed,globalsettings.jumpVelocity,Gameobject(&game.collisionList,sf::Vector2<float>(200,64),0,sf::Vector2<float>(32,32),true,&texturemap.at("Square"),false,true,globalsettings.gravity,globalsettings.playerFriction,0, sf::Vector2<float>(50,0)));
    game.objectList.push_back(&player);
    game.mainCamera.SetObjectToFollow(&player, 2);

    Enemy enemy = Enemy(10,100,Gameobject(&game.collisionList,sf::Vector2<float>(120,-200),0,sf::Vector2<float>(32,32),true,&texturemap.at("Noomba"),false,true,700,0,0, sf::Vector2<float>(0,0)));
    game.objectList.push_back(&enemy);

#pragma region WorldGen
    //Tile types
    std::vector<tile> tileTypes = {
        tile("Air",10,Gameobject(nullptr,sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),false,nullptr,true,false,0,0,0, sf::Vector2<float>(0,0))),
        tile("Dirt",10,Gameobject(&game.collisionList,sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),true,&texturemap.at("Dirt"),true,true,globalsettings.gravity,1,0.2,sf::Vector2<float>(0,0))),
        tile("Stone",10,Gameobject(&game.collisionList,sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),true,&texturemap.at("Stone"),true,true,globalsettings.gravity,1,0.2,sf::Vector2<float>(0,0))),
        tile("Gold",10,Gameobject(&game.collisionList,sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),true,&texturemap.at("Gold"),true,true,globalsettings.gravity,1,0.2,sf::Vector2<float>(0,0))),
        tile("Bedrock",1000000,Gameobject(&game.collisionList,sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),true,&texturemap.at("Bedrock"),true,true,globalsettings.gravity,1,0.2,sf::Vector2<float>(0,0))),

    };

    //This map is used to determine the rough shape of the terain and places dirt and stone there
    //Int points to the type of tile to spawn if the noise is above the float the tile is used
    std::map<int, float> worldgenMap{
        {0,0.f},
        {1,0.5f},
        {2,0.6f},
    };

    //This map is used to add more detail to the terrain by replacing certain tiles with other tiles based on noise
    //Int points to type to replace if the noise is above the float, the second int dictates which tile to place there
    std::map<int, std::pair<float, int>> replacementLayerMap{
        {2, std::pair<float, int>(0.75f, 3)},
    };

    level world = level(globalsettings.tileSize,globalsettings.worldSize, tileTypes, 4, worldgenMap,replacementLayerMap);
    
    for (std::vector<tile>& tilecolomn : world.tiles) {
        for (tile& _tile : tilecolomn) {
            if (_tile.tileName != "Air") {
                game.objectList.push_back(&_tile);
                game.collisionList.push_back(&_tile.sprite);
            }
        }
    }

#pragma endregion

#pragma region UI

    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("Fonts/RubikScribble-Regular.ttf")) {
        std::cout << "Failed to load font";
    }
    // select the font
    text.setFont(font); // font is a sf::Font

    // set the string to display
    text.setString("Hello world");

    // set the character size
    text.setCharacterSize(24); // in pixels, not points!

    // set the color
    text.setFillColor(sf::Color::White);

    // set the text style
    text.setStyle(sf::Text::Bold);
    game.uiElements.push_back(&text);


#pragma endregion

    while (window.isOpen())
    {

        //Run the game loop
        game.OnEvents();
        game.OnLoop(window);
        game.OnRender(window);


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        window.display();
        window.clear();

    }

    return 0;
}

