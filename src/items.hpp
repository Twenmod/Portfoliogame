#ifndef ITEMS_H
#define ITEMS_H
#include "gameobject.hpp"
#include <SFML/System/String.hpp>

class treasureItem : public Gameobject {
    public:
        treasureItem(); // Default constructor
        treasureItem(int _value,float startVelocity, Gameobject itemObject);
        int value;
        float startVelocity;
        void PickUp();
        void CalculatePhysics(std::vector<chunk*> chunkList);
};




#endif