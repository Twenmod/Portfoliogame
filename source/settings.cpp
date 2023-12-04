#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include "settings.hpp"

float settings::playerMoveSpeed = 200;
float settings::playerAcceleration = 30;
float settings::playerStopAcceleration = 40; 
float settings::playerAirAccelerationMultiplier = 0.5;
float settings::playerFriction = 0.1; 
float settings::groundedCheckOffset = 5;
float settings::jumpVelocity = 300;

float settings::gravity = 700;
float settings::jumpGravity = 500;

sf::Keyboard::Key settings::left = sf::Keyboard::A;
sf::Keyboard::Key settings::right = sf::Keyboard::D;
sf::Keyboard::Key settings::jump = sf::Keyboard::Space;

