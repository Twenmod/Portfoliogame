#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include "player.hpp"
#include "gameobject.hpp"
#include "SFMLMath.hpp"
#include "settings.hpp"


//Constructor
Player::Player(float _walkSpeed, float _jumpVelocity, Gameobject playerObject) : Gameobject(playerObject) {
    walkSpeed = _walkSpeed;
    jumpVelocity = _jumpVelocity;
}

void Player::OnLoop(sf::Time deltaTime, std::vector<sf::Sprite *> collisionList) {
    sf::Vector2<float> velocity = Gameobject::GetVelocity();
    
    float acceleration = settings::playerAcceleration;
    if (!grounded) acceleration *= settings::playerAirAccelerationMultiplier; 

    if (sf::Keyboard::isKeyPressed(settings::right)) {
        velocity.x += acceleration;
        if (velocity.x > walkSpeed)
            velocity.x = walkSpeed;
    }
    else if (sf::Keyboard::isKeyPressed(settings::left)) {
        velocity.x -= acceleration;
        if (velocity.x < -walkSpeed)
            velocity.x = -walkSpeed;
    }else {
        acceleration = settings::playerStopAcceleration;
        if (!grounded)
            acceleration *= settings::playerAirAccelerationMultiplier;

        if (velocity.x > settings::playerStopAcceleration)
            velocity.x -= settings::playerStopAcceleration;
        else if (velocity.x < -settings::playerStopAcceleration)
            velocity.x += settings::playerStopAcceleration;
    }

    if (sf::Keyboard::isKeyPressed(settings::jump) && grounded) {
        velocity.y = -jumpVelocity;
        grounded = false;
    }


    Gameobject::SetVelocity(velocity);
    
    //Calculate Base gameobject physics 
    Gameobject::OnLoop(deltaTime, collisionList);
}

void Player::CalculatePhysics(sf::Time deltaTime, std::vector<sf::Sprite*> collisionList) {

    float currentGravity = gravity;
    if(sf::Keyboard::isKeyPressed(settings::jump) && velocity.y < 0)
        currentGravity = settings::jumpGravity;

    velocity.y += currentGravity * deltaTime.asSeconds();

    //Collision detection

    bool test = false;

    for (sf::Sprite* other : collisionList) {
        if (&sprite != other) {
            sf::FloatRect spriteRect = sprite.getGlobalBounds();
            sf::FloatRect otherRect = other->getGlobalBounds();


            sf::Vector2<float> relativeVelocity = velocity;

            sf::Vector2<float> normal(0,0);

            //Simplify rect sides
            float spriteBottom = spriteRect.top + spriteRect.height;
            float otherBottom = otherRect.top + otherRect.height;
            float spriteRight = spriteRect.left + spriteRect.width;
            float otherRight = otherRect.left + otherRect.width;

            //Test all sides
            bool bottominsideother = spriteBottom <= otherBottom && spriteBottom >= otherRect.top;
            bool topinsideother = spriteRect.top >= otherRect.top && spriteRect.top <= otherBottom;
            bool leftinsideother = spriteRect.left >= otherRect.left && spriteRect.left <= otherRight;
            bool rightinsideother = spriteRight >= otherRect.left && spriteRight <= otherRight;


            grounded = spriteBottom+settings::groundedCheckOffset <= otherBottom && spriteBottom+settings::groundedCheckOffset >= otherRect.top;


            /*Debug visualizer
            std::cout << "\n  T:"<< topinsideother << "\n"
            << leftinsideother << " o " << rightinsideother << "\n"
            << "  B:" << bottominsideother << "\n"; 
            */

            //Find side, set normal and set outside of other
            if (leftinsideother && !rightinsideother && topinsideother && bottominsideother) {normal = sf::Vector2<float>(-1.f,  0.f); position.x = otherRect.left+spriteRect.width; } /* left */
            if (topinsideother && !bottominsideother && (leftinsideother || rightinsideother)) {normal = sf::Vector2<float>( 0.f, 1.f); position.y = otherBottom+spriteRect.height; } /* bottom */
            if (bottominsideother && !topinsideother && (leftinsideother || rightinsideother)) {normal = sf::Vector2<float>( 0.f,  -1.f); position.y = otherRect.top-spriteRect.height; } /* top */
            if (rightinsideother && !leftinsideother && topinsideother && bottominsideother) {normal = sf::Vector2<float>( 1.f,  0.f); position.x = otherRight-spriteRect.width; } /* right */

            //Negate velocity / bounce
            float totalVelocity = -1*(normal*relativeVelocity);
            //std::cout << "\nOldvelo:"<<velocity.y<<"\nAddVelo: " << totalVelocity;

            //Apply velocity;
            bool normalNegative = normal.y < 0 || normal.x < 0;
            bool addNegative = totalVelocity < 0;
            if ((normalNegative && !addNegative) || (addNegative && !normalNegative))
                velocity += normal * totalVelocity;

            //std::cout << "\n Normal: " << normal.y;
            if (normal != sf::Vector2<float>(0,0)) {
                velocity += -relativeVelocity*friction;
                test = true;
            }
        }
    }
    colliding = test;

   // std::cout << "\nnewvelo:"<<velocity.y;


    //Scale the velocity to deltaTime to get consistent velocity across framerates
    position += velocity*deltaTime.asSeconds();

};