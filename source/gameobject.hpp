#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "gmath.hpp"

class Gameobject {
    public:
        ///Transform

        Vector2 position;
        float rotation;
        Vector2 scale;

        ///Physics

        bool isStatic;
        float gravity;
        float drag;

        Gameobject(Vector2 _position, float _rotation, Vector2 _scale);
        void OnLoop();
    private:
        Vector2 velocity;
        void CalculateVelocity();
};

#endif