#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>
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
#include <iostream>
#include <map>
#include <vector>
#include "math.hpp"

sf::Vector2<int> resolution(200,200);

Settings globalsettings = Settings();


//The game itself
class app {
    public:
        Camera mainCamera;


        //Lists of the objects split for optimisation
        std::vector<Gameobject*> objectList;
        std::vector<sf::Sprite*> collisionList;
        
        sf::Clock gameClock; 

        sf::RenderWindow* gameWindow;

        //Constructer/Init
        app(sf::RenderWindow &window) {
            gameWindow = &window;
            mainCamera = generateCamera(sf::Vector2<float>(0,0), sf::Vector2<float>(1,1), sf::Vector2<float>(500,500));
        }

        Camera generateCamera(sf::Vector2<float> startPosition, sf::Vector2<float> scale, sf::Vector2<float> cullDistance) {
            return Camera(startPosition,scale,gameWindow->getSize(),cullDistance);
        }

        void OnEvents() {
            for (Gameobject* obj : objectList) {
                obj->OnEvent();
            }
        };
        void OnLoop(sf::RenderWindow &window) {
            //Get time elapsed since last frame
            sf::Time deltaTime = gameClock.restart();
            
            //Calculate camera
            mainCamera.OnLoop(deltaTime, window);
            //Calculate objects
            for (Gameobject* obj : objectList) {
                obj->OnLoop(deltaTime, collisionList);
            }

        };
        void OnRender(sf::RenderWindow &window) {
            mainCamera.Render(window,objectList);
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

    //Get resolution
    resolution = sf::Vector2<int>(sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height);

    //Initialize window
    sf::RenderWindow window(sf::VideoMode((int)resolution.x,(int)resolution.y), "Minergame", sf::Style::Close);

    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    //Map of the games textures

    std::map<std::string, sf::Texture> texturemap = {
        {"Square", addTexture("Sprites/Square.jpg")},
        {"Dirt", addTexture("Sprites/Tiles/Dirt.png")},
        {"Stone",addTexture("Sprites/Tiles/Stone.png")},
        {"Noomba",addTexture("Sprites/noomba.png")}
    };

    //Load game
    app game(window);

    //Load level
    Player player = Player(globalsettings.playerMoveSpeed,globalsettings.jumpVelocity,Gameobject(game.collisionList,sf::Vector2<float>(55,-300),0,sf::Vector2<float>(32,32),true,&texturemap.at("Square"),false,true,globalsettings.gravity,globalsettings.playerFriction,0, sf::Vector2<float>(50,0)));
    game.objectList.push_back(&player);
    game.mainCamera.SetObjectToFollow(&player, 2);

    Enemy enemy = Enemy(10,100,Gameobject(game.collisionList,sf::Vector2<float>(120,-200),0,sf::Vector2<float>(32,32),true,&texturemap.at("Noomba"),false,true,700,0,0, sf::Vector2<float>(0,0)));
    game.objectList.push_back(&enemy);


    std::vector<tile> tileTypes = {
        tile("Dirt",10,Gameobject(game.collisionList,sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),true,&texturemap.at("Dirt"),true,true,globalsettings.gravity,1,0.2,sf::Vector2<float>(0,0))),
        tile("Stone",10,Gameobject(game.collisionList,sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),true,&texturemap.at("Stone"),true,true,globalsettings.gravity,1,0.2,sf::Vector2<float>(0,0)))

    };

    //Int points to the type of tile to spawn if the noise is above the float the tile is used
    std::map<int, float> worldgenMap{
        {0,0.5f},
        {1,0.6f}
    };


    level world = level(globalsettings.tileSize,globalsettings.worldSize, tileTypes, worldgenMap);
    
    for (std::vector<tile>& tilecolomn : world.tiles) {
        for (tile& _tile : tilecolomn) {
            game.objectList.push_back(&_tile);
            game.collisionList.push_back(&_tile.sprite);
        }
    }


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

