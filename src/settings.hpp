#include "player.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sys/types.h>


class Settings {
        public:
        Settings();

//Audio
        float audioFalloff;

//Video
        int frameRateLimit;
        bool vSync;
        sf::Vector2<int> windowSize;
        sf::Vector2<int> cullSize;
        sf::Color backgroundColor;
        bool fullscreen;

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
        float jumpVelocity;
        float cayoteTime;

        float climbSpeed;

        float gravity;
        float jumpGravity;

        float fallDamage;
        float fallDamageThreshold;

        float attackRange;
        float attackDamage;
        float attackInterval;
        float attackDelay;

        float whipAttackRange;
        float whipAttackDamage;
        float whipAttackInterval;
        float whipAttackDelay;

        int playerRopeSize;

//World generation
        int chunkSize;
        int chunkLoadDistance;
        float tileSize;
        sf::Vector2<int> worldSize;
        sf::Vector2<float> noiseScale;
        sf::Vector2<float> oreNoiseScale;
        int octaves;
        float persistence;
        sf::Vector2<int> amountOfEnemies;
        sf::Vector2<int> amountOfObjects;
        int optionalObjectSpawnMaxIterations;
        int worldSeed;


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
