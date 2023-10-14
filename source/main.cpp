#include <SFML/Graphics.hpp>
#include <iostream>
#include "gmath.hpp"
#include "gameobject.hpp"

sf::Vector2<int> resolution(200,200);

//The game itself
class app {
    public:
        //Lists of the objects split for optimisation
        std::vector<Gameobject> objectList;
        std::vector<Gameobject> collisionList;

        sf::Clock gameClock; 

        void OnEvents() {
            for (Gameobject& obj : objectList) {
                obj.OnEvent();
            }
        };
        void OnLoop() {

            //Get time elapsed since last frame
            sf::Time deltaTime = gameClock.restart();

            for (Gameobject& obj : objectList) {
                obj.OnLoop(deltaTime);
            }
        };
        void OnRender(sf::RenderWindow &window) {
            for (Gameobject& obj : objectList) {
                obj.OnRender();

                //Only render if object contains a sprite
                if (obj.hasSprite) {
                    std::cout << obj.sprite.getTexture()->getSize().x << "\n";
                    window.draw(obj.sprite);
                }
            }
        };

        Gameobject createObject(sf::Vector2<float> _position, float _rotation,sf::Vector2<float> _scale, bool _hasSprite, sf::Texture* _texture, bool _isStatic, bool _hasCollision, float _gravity, float _drag) {
            Gameobject obj(_position, _rotation, _scale, _hasSprite, _texture, _isStatic, _hasCollision, _gravity, _drag);
            if (obj.hasCollision) {
                collisionList.push_back(obj);
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
    std::cout << text.getSize().x << "\n";
    return text;
}

int main()
{

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
    game.createObject(sf::Vector2<float>(5,0),0,sf::Vector2<float>(0.2,0.1),true,&texturemap.at("Nobitches"),false,true,2,0);
    game.createObject(sf::Vector2<float>(5,100),0,sf::Vector2<float>(0.2,0.1),true,&texturemap.at("Nobitches"),true,true,2,0);


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

