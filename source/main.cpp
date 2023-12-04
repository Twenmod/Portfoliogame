#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "gameobject.hpp"
#include "camera.hpp"
#include "player.hpp"
#include "settings.hpp"
#include <SFML/Window/WindowStyle.hpp>
#include <algorithm>
#include <iostream>


sf::Vector2<int> resolution(200,200);

//The game itself
class app {
    public:
        Camera mainCamera;


        //Lists of the objects split for optimisation
        std::vector<Gameobject*> objectList;
        std::vector<sf::Sprite*> collisionList;

        sf::Clock gameClock; 

        //Constructer/Init
        app() {
            mainCamera = generateCamera(sf::Vector2<float>(0,0), sf::Vector2<float>(1,1), sf::Vector2<float>(500,500));
        }

        Camera generateCamera(sf::Vector2<float> startPosition, sf::Vector2<float> scale, sf::Vector2<float> cullDistance) {
            return Camera(startPosition,scale,cullDistance);
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
    sf::RenderWindow window(sf::VideoMode((int)resolution.x,(int)resolution.y), "Minergame", sf::Style::Fullscreen);

    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    //Map of the games textures

    std::map<std::string, sf::Texture> texturemap = {
        {"Square", addTexture("Sprites/Square.jpg")},
        {"Dirt", addTexture("Sprites/Tiles/Dirt.png")}
    };




    //Load game
    app game;

    //Load level
    Player player = Player(settings::playerMoveSpeed,settings::jumpVelocity,Gameobject(game.collisionList,sf::Vector2<float>(55,0),0,sf::Vector2<float>(32,32),true,&texturemap.at("Square"),false,true,settings::gravity,settings::playerFriction,0, sf::Vector2<float>(50,0)));
    game.objectList.push_back(&player);
    game.mainCamera.SetObjectToFollow(&player, 2);
    Gameobject obj2 = Gameobject(game.collisionList, sf::Vector2<float>(50,300),0,sf::Vector2<float>(10,10),true,&texturemap.at("Dirt"),true,true,2,0,0,sf::Vector2<float>(0,0));
    game.objectList.push_back(&obj2);

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

        window.display();
        window.clear();

    }

    return 0;
}

