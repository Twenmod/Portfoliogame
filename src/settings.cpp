#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <iostream>
#include <json/value.h>
#include <vector>
#include "settings.hpp"
#include <fstream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include <time.h>

//Read all configs from settings.json
Settings::Settings() {

    try {
        std::ifstream file("settings.json");
        Json::Value settingsfile;
        Json::Reader reader;
        reader.parse(file, settingsfile);

        //Video
        frameRateLimit = settingsfile["frameRateLimit"].asInt();
        vSync = settingsfile["vSync"].asBool();
        windowSize = sf::Vector2<int>(settingsfile["windowSize"]["width"].asInt(), settingsfile["windowSize"]["height"].asInt());
        cullSize = sf::Vector2<int>(settingsfile["cullSize"]["width"].asInt(), settingsfile["cullSize"]["height"].asInt());
        backgroundColor = sf::Color(settingsfile["backgroundColor"]["r"].asInt(), settingsfile["backgroundColor"]["g"].asInt(), settingsfile["backgroundColor"]["b"].asInt());

        //physics
        playerPhysicsStepDistance = settingsfile["playerPhysicsStepDistance"].asFloat();
        playerMaxPhysicsIterations = settingsfile["playerMaxPhysicsIterations"].asInt();
        maxDeltaTime = settingsfile["maxDeltaTime"].asFloat();

        //Player
        playerMoveSpeed = settingsfile["playerMoveSpeed"].asFloat();
        playerAcceleration = settingsfile["playerAcceleration"].asFloat();
        playerStopAcceleration = settingsfile["playerStopAcceleration"].asFloat();
        playerAirAccelerationMultiplier = settingsfile["playerAirAccelerationMultiplier"].asFloat();;
        // Reduces player speed when moving across ground value between 0-1
        playerFriction = settingsfile["playerFriction"].asFloat();
        // Moves groundcheck x pixels under players bottom rect
        jumpVelocity = settingsfile["jumpVelocity"].asFloat();
        cayoteTime = settingsfile["cayoteTime"].asFloat();

        fallDamage = settingsfile["fallDamage"].asFloat();
        fallDamageThreshold = settingsfile["fallDamageThreshold"].asFloat();

        gravity = settingsfile["gravity"].asFloat();
        // Gravity that is used when holding jump and moving up (to give more jump control)
        jumpGravity = settingsfile["jumpGravity"].asFloat();

        attackRange = settingsfile["attackRange"].asFloat();
        attackDamage = settingsfile["attackDamage"].asFloat();
        attackInterval = settingsfile["attackInterval"].asFloat();
        attackDelay = settingsfile["attackDelay"].asFloat();

        whipAttackRange = settingsfile["whipAttackRange"].asFloat();
        whipAttackDamage = settingsfile["whipAttackDamage"].asFloat();
        whipAttackInterval = settingsfile["whipAttackInterval"].asFloat();
        whipAttackDelay = settingsfile["whipAttackDelay"].asFloat();


        //World Generation

        worldSeed = settingsfile["worldSeed"].asInt();



        chunkSize = settingsfile["chunkSize"].asInt();
        chunkLoadDistance = settingsfile["chunkLoadDistance"].asInt();
        tileSize = settingsfile["tileSize"].asInt();
        worldSize = sf::Vector2<int>(settingsfile["worldSize"]["width"].asInt(), settingsfile["worldSize"]["height"].asInt());
        noiseScale = sf::Vector2<float>(settingsfile["noiseScale"]["width"].asFloat(), settingsfile["noiseScale"]["height"].asFloat());
        oreNoiseScale = sf::Vector2<float>(settingsfile["oreNoiseScale"]["width"].asFloat(), settingsfile["oreNoiseScale"]["height"].asFloat());
        octaves = settingsfile["octaves"].asInt();
        persistence = settingsfile["persistence"].asFloat();
        amountOfEnemies = sf::Vector2<int>(settingsfile["amountOfEnemies"]["min"].asInt(), settingsfile["amountOfEnemies"]["max"].asInt());
        amountOfObjects = sf::Vector2<int>(settingsfile["amountOfObjects"]["min"].asInt(), settingsfile["amountOfObjects"]["max"].asInt());
        optionalObjectSpawnMaxIterations = settingsfile["optionalObjectSpawnMaxIterations"].asInt();
        

        //Keybinds
        left = sf::Keyboard::A;
        right = sf::Keyboard::D;
        jump = sf::Keyboard::Space;

        attackRight = sf::Keyboard::Right;
        attackLeft = sf::Keyboard::Left;
        attackUp = sf::Keyboard::Up;
        attackDown = sf::Keyboard::Down;

    }
    catch (const std::exception& e) {
        std::cerr << "Error occurred during Settings initialization: " << e.what() << std::endl;
    }
};