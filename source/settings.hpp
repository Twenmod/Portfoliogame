#include "player.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sys/types.h>

//Most game settings are controlled here for ease of use

namespace settings {

        extern float playerMoveSpeed;
        extern float playerAcceleration;
        extern float playerStopAcceleration;
        extern float playerAirAccelerationMultiplier;
        extern float playerFriction;
        extern float groundedCheckOffset;
        extern float jumpVelocity;

        extern float gravity;
        extern float jumpGravity;

        //World generation
        extern float noiseScale;
        extern float octaves;
        extern float persistence;


        //Input
        extern sf::Keyboard::Key left;
        extern sf::Keyboard::Key right;
        extern sf::Keyboard::Key jump;

};

