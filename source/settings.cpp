#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include "settings.hpp"

float settings::playerMoveSpeed = 200;
float settings::playerAcceleration = 800;
float settings::playerStopAcceleration = 1000; 
float settings::playerAirAccelerationMultiplier = 0.5;
// Reduces player speed when moving across ground value between 0-1
float settings::playerFriction = 0.1; 
// Moves groundcheck x pixels under players bottom rect
float settings::groundedCheckOffset = 5;
float settings::jumpVelocity = 300;

float settings::gravity = 700;
// Gravity that is used when holding jump and moving up (to give more jump control)
float settings::jumpGravity = 500;

sf::Keyboard::Key settings::left = sf::Keyboard::A;
sf::Keyboard::Key settings::right = sf::Keyboard::D;
sf::Keyboard::Key settings::jump = sf::Keyboard::Space;

