#include <SFML/Graphics.hpp>
#include <iostream>
#include "gmath.hpp"
#include "gameobject.hpp"

sf::Vector2<int> resolution(200,200);

//The game itself
class app {
    public:

        std::vector<Gameobject> objectList;

        void OnEvents() {
            for (Gameobject obj : objectList) {
                obj.OnEvent();
            }
        };
        void OnLoop() {

            for (Gameobject obj : objectList) {
                obj.OnLoop();
            }
        };
        void OnRender(sf::RenderWindow &window) {
            for (Gameobject obj : objectList) {
                obj.OnRender();

                //Only render if object contains a sprite
                if (obj.hasSprite) {
                    window.draw(obj.sprite);
                }
            }
        };

};


int main()
{

    //Initialisation
    sf::RenderWindow window(sf::VideoMode((int)resolution.x,(int)resolution.y), "Minergame");

    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    app game;

    Gameobject obj(sf::Vector2<float>(5,0),0,sf::Vector2<float>(0.2,0.1),true,"Sprites/nobitches.png",false,10000,0);
    game.objectList.push_back(obj);

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

