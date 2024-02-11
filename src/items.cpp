#include "items.hpp"
#include "gameobject.hpp"
#include "worldgen.hpp"
#include <SFML/System/Vector2.hpp>
#include <cstdlib>

treasureItem::treasureItem() : Gameobject() {

}

treasureItem::treasureItem(int _value, float _startVelocity, Gameobject itemObject) : Gameobject(itemObject)
{
    value = _value;
    startVelocity = _startVelocity;
}
void treasureItem::PickUp() {
    //remove itself from the chunk
    Gameobject::~Gameobject();
    treasureItem::~treasureItem();
}