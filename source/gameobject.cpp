#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <vector>
#include "gameobject.hpp"
#include "SFMLMath.hpp"

sf::Texture emptyimage();

//Constructor
Gameobject::Gameobject(sf::Vector2<float> _position, float _rotation = 0,sf::Vector2<float> _scale =sf::Vector2<float>(1,1), bool _hasSprite = false, sf::Texture* _texture = nullptr, bool _isStatic = false, bool _hasCollision = true, float _gravity = 10, float _drag = 0) {

    position = _position;
    rotation = _rotation;
    scale = _scale;

    ///Sprite

    hasSprite = _hasSprite;
    if (hasSprite) {

        sprite.setTexture(*_texture);
        sprite.setPosition(position);
        sprite.setRotation(rotation);
        SetScale(_scale);
    }

    velocity = sf::Vector2<float>(5,0);

    //Physics
    isStatic = _isStatic;
    hasCollision = _hasCollision;
    gravity = _gravity;
    drag = _drag;



};

//Sets scale of the sprite
void Gameobject::SetScale(sf::Vector2<float> scaleToSet) {
    if (hasSprite)
        sprite.setScale(scaleToSet);
}

//Called before loop after inputs
void Gameobject::OnEvent() {

}

//Called every frame
void Gameobject::OnLoop(sf::Time deltaTime, std::vector<sf::Sprite*> collisionList) {
    if (!isStatic) {
        CalculatePhysics(deltaTime, collisionList);
    }
};

//Called just before render
void Gameobject::OnRender() {
    if(hasSprite) {
        sprite.setPosition(position);
        sprite.setRotation(rotation);
    }
};

void Gameobject::CalculatePhysics(sf::Time deltaTime, std::vector<sf::Sprite*> collisionList) {

    velocity.y += gravity;

    //Collision detection

    bool test = false;

    for (sf::Sprite* other : collisionList) {
        if (&sprite != other) {
            sf::FloatRect spriteRect = sprite.getGlobalBounds();
            sf::FloatRect otherRect = other->getGlobalBounds();
            //Colliding!
            //Test all sides

            sf::Vector2<float> relativeVelocity = velocity;

            sf::Vector2<float> dirfrac;

            //Shoot a ray with the current velocity towards the rect of other
            dirfrac.x = 1.0f / (relativeVelocity.x);
            dirfrac.y = 1.0f / (relativeVelocity.y);
            
            // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
            sf::Vector2<float> lb(otherRect.left,otherRect.top+otherRect.height);
            sf::Vector2<float> rt(otherRect.left+otherRect.width,otherRect.top);

            float t1 = (lb.x - position.x)*dirfrac.x;
            float t2 = (rt.x - position.x)*dirfrac.x;
            float t3 = (lb.y - position.y)*dirfrac.y;
            float t4 = (rt.y - position.y)*dirfrac.y;

            float tmin = std::max(std::min(t1, t2), std::min(t3, t4));
            float tmax = std::min(std::max(t1, t2), std::max(t3, t4));

            float t;

            // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
            // if tmin > tmax, ray doesn't intersect AABB

            if (tmax < 0 || tmin > tmax)
            {
                t = tmax;
                test = false;
            }else {
            t = tmin;

            sf::Vector2<float> normal;
            if (t == t1) normal = sf::Vector2<float>(-1.f,  0.f); /* left */
            if (t == t3) normal = sf::Vector2<float>( 0.f, -1.f); /* bottom */
            if (t == t4) normal = sf::Vector2<float>( 0.f,  1.f); /* top */
            if (t == t2) normal = sf::Vector2<float>( 1.f,  0.f); /* right */

            float totalVelocity = -(1+bounciness)*(normal*relativeVelocity);

            velocity += normal * totalVelocity;
            }

            test = true;

        }
    }
    colliding = test;


    //Scale the velocity to deltaTime to get consistent velocity across framerates
    position += velocity*deltaTime.asSeconds();

};