#include <SFML/Graphics.hpp>
#include "headers/gmath.h"

vector2 resolution(200,200);

//The game itself
class app {
    public:
        void on_events() {

        };
        void on_loop() {

        };
        void on_render() {

        };
};


int main()
{

    sf::RenderWindow window(sf::VideoMode((int)resolution.x,(int)resolution.y), "Minergame");

    app game;

    while (window.isOpen())
    {


        //Run the game loop
        game.on_events();
        game.on_loop();
        game.on_render();


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.display();
    }

    return 0;
}

