#include <SFML/Graphics.hpp>
#include "gmath.hpp"

Vector2 resolution = Vector2(200,200);

//The game itself
class app {
    public:
        void OnEvents() {

        };
        void OnLoop() {

        };
        void OnRender() {

        };
};


int main()
{
    //Initialisation
    sf::RenderWindow window(sf::VideoMode((int)resolution.x,(int)resolution.y), "Minergame");

    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    app game;

    sf::Texture texture;
    // load a 32x32 rectangle that starts at (10, 10)
    if (!texture.loadFromFile("/Sprites/Koen.png", sf::IntRect(10, 10, 32, 32)))
    {
        // error...
    }

    // update a texture from an array of pixels
    sf::Uint8* pixels = new sf::Uint8[200 * 200 * 4]; // * 4 because pixels have 4 components (RGBA)

    texture.update(pixels);

    // update a texture from a sf::Image
    sf::Image image;
    texture.update(image);

    // update the texture from the current contents of the window
    texture.update(window);

    sf::Sprite sprite;
    sprite.setTexture(texture);

    while (window.isOpen())
    {


        window.draw(sprite);


        //Run the game loop
        game.OnEvents();
        game.OnLoop();
        game.OnRender();


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

