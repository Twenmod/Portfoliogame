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
        std::vector<Gameobject> objectList;
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
            for (Gameobject& obj : objectList) {
                obj.OnEvent();
            }
        };
        void OnLoop() {
            //Get time elapsed since last frame
            sf::Time deltaTime = gameClock.restart();

            //Calculate camera
            mainCamera.OnLoop(deltaTime);
            //Calculate objects
            for (Gameobject& obj : objectList) {
                obj.OnLoop(deltaTime, collisionList);
            }

        };
        void OnRender(sf::RenderWindow &window) {
            mainCamera.Render(window,objectList);
        };

        Gameobject createObject(sf::Vector2<float> _position = sf::Vector2<float>(0,0), float _rotation = 0, sf::Vector2<float> _scale = sf::Vector2<float>(1,1), bool _hasSprite = true, sf::Texture* _texture = nullptr, bool _isStatic = false, bool _hasCollision = true, float _gravity = 10, float _drag = 1, sf::Vector2<float> _startVelocity = sf::Vector2<float>(0,0)) {
            Gameobject obj(_position, _rotation, _scale, _hasSprite, _texture, _isStatic, _hasCollision, _gravity, _drag, _startVelocity);
            if (obj.hasCollision && obj.hasSprite) {
                collisionList.push_back(&obj.sprite);
            }
            objectList.push_back(obj);

            return obj;
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
    game.createObject(sf::Vector2<float>(55,0),0,sf::Vector2<float>(0.2,0.2),true,&texturemap.at("Nobitches"),false,true,20,0, sf::Vector2<float>(50,0));
    game.mainCamera.SetObjectToFollow(&game.objectList.at(game.objectList.size()-1), 1);
    game.createObject(sf::Vector2<float>(150,300),0,sf::Vector2<float>(2,2),true,&texturemap.at("Nobitches"),true,true,2,0);


    while (window.isOpen())
    {

        //Run the game loop
        game.OnEvents();
        game.OnLoop();
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

