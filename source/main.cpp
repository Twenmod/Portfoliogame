#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "gameobject.hpp"
#include "camera.hpp"
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
    sf::RenderWindow window(sf::VideoMode((int)resolution.x,(int)resolution.y), "Minergame");

    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    //Map of the games textures

    std::map<std::string, sf::Texture> texturemap = {
        {"Nobitches", addTexture("Sprites/nobitches.png")}
    };


    //Load game
    app game;

    //Load level
    Gameobject obj = Gameobject(game.collisionList,sf::Vector2<float>(55,0),0,sf::Vector2<float>(0.2,0.2),true,&texturemap.at("Nobitches"),false,true,20,0, sf::Vector2<float>(50,0));
    game.objectList.push_back(&obj);
    game.mainCamera.SetObjectToFollow(&obj, 5);
    //game.createObject(sf::Vector2<float>(50,300),0,sf::Vector2<float>(2,2),true,&texturemap.at("Nobitches"),true,true,2,0);


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

