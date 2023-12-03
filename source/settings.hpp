#ifndef SETTINGS_H
#define SETTINGS_H
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

//Most game settings are controlled here for ease of use

class settings {

    public:
        settings();
        float playerMoveSpeed = 50;
};

#endif