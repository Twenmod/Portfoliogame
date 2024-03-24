#include "rope.hpp"
#include "globals.hpp"
#include "settings.hpp"
#include "worldgen.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

Rope::Rope(int _length, float _growDelay, sf::Texture* _middleTexture, Gameobject _baseObject) : Gameobject(_baseObject) {
	length = _length;
	delay = _growDelay;
	growDelay = _growDelay;
	middleTexture = _middleTexture;
};
void Rope::OnLoop(std::vector<chunk*> chunkList) {
	if (length <= 0) return;
	
	if (delay <= 0 && !hasGrown) {
		//Grow rope

		hasGrown = true;
		
		///Check for free space underneath current rope
		sf::Rect checkRect(sprite.getGlobalBounds());
		checkRect.top += globalsettings.tileSize;

		for (chunk* _chunk : chunkList)
		{
			if (_chunk->collisionObjects.size() == 0) continue;
			for (Gameobject* otherobject : _chunk->collisionObjects)
			{
				if (!otherobject->hasCollision)
					continue;

				sf::Sprite* other = &otherobject->sprite;
				if (&sprite == other) continue;

				if (checkRect.intersects(other->getGlobalBounds()))
				{
					//Colliding in space so cant spawn 
					return;
				}
			}
		}

		//Spawn new rope piece
		Rope* newRope = new Rope(length - 1, growDelay, middleTexture, *dynamic_cast<Gameobject*>(this)); // Create a copy of self with 1 less length
		newRope->position.y += globalsettings.tileSize; // Move down

		//Get corresponding chunk
		sf::Vector2i chunkPos(
			int(newRope->position.x / globalsettings.tileSize / globalsettings.chunkSize),
			int(newRope->position.y / globalsettings.tileSize / globalsettings.chunkSize)
		);
		//Check if inside world
		if ((chunkPos.x >= 0 || chunkPos.x < globalsettings.worldSize.x) || (chunkPos.y >= 0 || chunkPos.y < globalsettings.worldSize.y)) {
			//Add to chunk
			globalChunkList[chunkPos.x][chunkPos.y]->objects.push_back(newRope);
			if (newRope->hasCollision)
				globalChunkList[chunkPos.x][chunkPos.y]->collisionObjects.push_back(newRope);
			
			//Change own texture to middle texture
			sprite.setTexture(*middleTexture);
		}
		else {
			//Otherwise cleanup
			delete newRope;
		}



	}else 
		delay -= deltaTime.asSeconds();

}