#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include "player.hpp"
#include "gameobject.hpp"
#include "SFMLMath.hpp"

//Constructor
Player::Player(float _walkSpeed, float _jumpVelocity, Gameobject playerObject) : Gameobject(playerObject) {
    walkSpeed = _walkSpeed;
    jumpVelocity = _jumpVelocity;
}

void Player::OnLoop(sf::Time deltaTime, std::vector<sf::Sprite *> collisionList) {
    sf::Vector2<float> velocity = Gameobject::GetVelocity();
    
    velocity.x = 10;

    Gameobject::SetVelocity(velocity);
    
    //Calculate Base gameobject physics 
    Gameobject::OnLoop(deltaTime, collisionList);
}