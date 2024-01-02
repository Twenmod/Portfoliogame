#include "items.hpp"
#include "gameobject.hpp"
#include "worldgen.hpp"

treasureItem::treasureItem(int _value, Gameobject itemObject) : Gameobject(itemObject)
{
    value = _value;
}
void treasureItem::PickUp() {
    //remove itself from the chunk
    currentChunk->objects.erase(std::remove(currentChunk->objects.begin(), currentChunk->objects.end(), this), currentChunk->objects.end());
    currentChunk->collisionObjects.erase(std::remove(currentChunk->collisionObjects.begin(), currentChunk->collisionObjects.end(), this), currentChunk->collisionObjects.end());

    treasureItem::~treasureItem();
}