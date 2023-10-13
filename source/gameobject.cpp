#include "gameobject.hpp"
#include "gmath.hpp"

//Constructor
Gameobject::Gameobject(Vector2 _position = Vector2(0,0), float _rotation = 0, Vector2 _scale = Vector2(1,1)) {
    position = _position;
    rotation = _rotation;
    scale = _scale;
};

//Called every frame
void Gameobject::OnLoop() {
    if (Gameobject::isStatic) {
    Gameobject::CalculateVelocity();
    }
};

void Gameobject::CalculateVelocity() {
    Gameobject::position.x += Gameobject::velocity.x;
    Gameobject::position.y += Gameobject::velocity.y;

    Gameobject::velocity.y -= Gameobject::gravity;
};