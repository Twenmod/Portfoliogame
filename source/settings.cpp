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

//Read all configs from settings.json
Settings::Settings() {
    std::ifstream file("settings.json");
    Json::Value settingsfile;
    Json::Reader reader;
    reader.parse(file, settingsfile);

    //Window
    frameRateLimit = settingsfile["frameRateLimit"].asInt();
    windowSize = sf::Vector2<int>(settingsfile["windowSize"]["width"].asInt(), settingsfile["windowSize"]["height"].asInt());
    cullSize = sf::Vector2<int>(settingsfile["cullSize"]["width"].asInt(), settingsfile["cullSize"]["height"].asInt());

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
    groundedCheckOffset = settingsfile["groundedCheckOffset"].asFloat();
    jumpVelocity = settingsfile["jumpVelocity"].asFloat();

    gravity = settingsfile["gravity"].asFloat();
    // Gravity that is used when holding jump and moving up (to give more jump control)
    jumpGravity = settingsfile["jumpGravity"].asFloat();


    //World Generation
    tileSize = settingsfile["tileSize"].asInt();
    worldSize = sf::Vector2<int>(settingsfile["worldSize"]["width"].asInt(), settingsfile["worldSize"]["height"].asInt());
    noiseScale = sf::Vector2<float>(settingsfile["noiseScale"]["width"].asFloat(), settingsfile["noiseScale"]["height"].asFloat());
    oreNoiseScale = sf::Vector2<float>(settingsfile["oreNoiseScale"]["width"].asFloat(), settingsfile["oreNoiseScale"]["height"].asFloat());
    octaves = settingsfile["octaves"].asInt();
    persistence = settingsfile["persistence"].asFloat();

    //Keybinds
    left =  sf::Keyboard::A;
    right = sf::Keyboard::D;
    jump = sf::Keyboard::Space;

};



