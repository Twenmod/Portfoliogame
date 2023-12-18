#include "player.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sys/types.h>


class Settings {
        public:
        Settings();
//Player
        float playerMoveSpeed;
        float playerAcceleration;
        float playerAirAccelerationMultiplier;
        float playerStopAcceleration;
        float playerFriction;
        float groundedCheckOffset;
        float jumpVelocity;

        float gravity;
        float jumpGravity;

//World generation
        float tileSize;
        sf::Vector2<int> worldSize;
        sf::Vector2<float> noiseScale;
        float octaves;
        float persistence;


//Input
        sf::Keyboard::Key left;
        sf::Keyboard::Key right;
        sf::Keyboard::Key jump;

};

extern Settings globalsettings;
