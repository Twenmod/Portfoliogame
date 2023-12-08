#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include "settings.hpp"

float settings::playerMoveSpeed = 150;
float settings::playerAcceleration = 1000;
float settings::playerStopAcceleration = 1500; 
float settings::playerAirAccelerationMultiplier = 0.8;
// Reduces player speed when moving across ground value between 0-1
float settings::playerFriction = 0.1; 
// Moves groundcheck x pixels under players bottom rect
float settings::groundedCheckOffset = 5;
float settings::jumpVelocity = 300; //300

float settings::gravity = 1500; // 1500
// Gravity that is used when holding jump and moving up (to give more jump control)
float settings::jumpGravity = 700; // 700

sf::Keyboard::Key settings::left = sf::Keyboard::A;
sf::Keyboard::Key settings::right = sf::Keyboard::D;
sf::Keyboard::Key settings::jump = sf::Keyboard::Space;

