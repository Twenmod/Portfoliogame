#ifndef ITEMS_H
#define ITEMS_H
#include "gameobject.hpp"
#include <SFML/System/String.hpp>

class treasureItem : public Gameobject {
    public:
        treasureItem(int _value, Gameobject itemObject);
        int value;

        void PickUp();
};




#endif