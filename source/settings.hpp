#include "player.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sys/types.h>


class Settings {
        public:
        Settings();
//Video
        int frameRateLimit;
        bool vSync;
        sf::Vector2<int> windowSize;
        sf::Vector2<int> cullSize;

//Physics
        float playerPhysicsStepDistance;
        int playerMaxPhysicsIterations;
        float maxDeltaTime;
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

        float attackRange;
        float attackDamage;
        float attackInterval;

//World generation
        int chunkSize;
        int chunkLoadDistance;
        float tileSize;
        sf::Vector2<int> worldSize;
        sf::Vector2<float> noiseScale;
        sf::Vector2<float> oreNoiseScale;
        float octaves;
        float persistence;
        sf::Vector2<int> amountOfEnemies;


//Input
        sf::Keyboard::Key left;
        sf::Keyboard::Key right;
        sf::Keyboard::Key jump;

        sf::Keyboard::Key attackRight;
        sf::Keyboard::Key attackLeft;
        sf::Keyboard::Key attackUp;
        sf::Keyboard::Key attackDown;

        

};

extern Settings globalsettings;
extern sf::Time deltaTime;