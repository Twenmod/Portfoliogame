#include "rope.hpp"
#include "globals.hpp"
#include "settings.hpp"
#include "worldgen.hpp"
#include "math.hpp"
#include "SFMLMath.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>


Rope::Rope() {

};
Rope::Rope(int _length, float _growDelay, std::vector<sf::Texture*> _middleTexture, Gameobject _baseObject, bool _isStart, std::vector<sf::Texture*> _startTexture) : Gameobject(_baseObject) {
	length = _length;
	delay = _growDelay;
	growDelay = _growDelay;
	middleTexture = _middleTexture;
	isStart = _isStart;
	startTexture = _startTexture;
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

                if (otherobject->objectName == "ropeProjectile" || otherobject->objectName == "Player")
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
			
			//Change own texture
			if (isStart) {
				sprite.setOrigin(sf::Vector2f(0, 32));
				sprite.setTexture(*startTexture[0], true);
			}
			else {
				sprite.setTexture(*middleTexture[rand() % middleTexture.size()]);
			}
		}
		else {
			//Otherwise cleanup
			delete newRope;
		}



	}else 
		delay -= deltaTime.asSeconds();

}

RopeProjectile::RopeProjectile() {

};
RopeProjectile::RopeProjectile(Rope _ropeObject, Gameobject projectileObject) : Gameobject(projectileObject) {
	ropeObject = _ropeObject;

}

void RopeProjectile::spawnRope(std::vector<chunk*> chunkList) {

    //Get rounded position so the projectile spawns in the grid
    sf::Vector2f roundedPosition = position;
    roundedPosition.x = std::floor(roundedPosition.x / globalsettings.tileSize) * globalsettings.tileSize;
    roundedPosition.y = (std::floor(roundedPosition.y / globalsettings.tileSize) + 1) * globalsettings.tileSize;

    ///Check for space
    sf::Rect checkRect(sprite.getGlobalBounds());
    checkRect.top = roundedPosition.y;
    checkRect.left = roundedPosition.x;

    for (chunk* _chunk : chunkList)
    {
        if (_chunk->collisionObjects.size() == 0) continue;
        for (Gameobject* otherobject : _chunk->collisionObjects)
        {
            if (!otherobject->hasCollision)
                continue;

            if (otherobject->objectName == "ropeProjectile" || otherobject->objectName == "Player")
                continue;

            sf::Sprite* other = &otherobject->sprite;
            if (&sprite == other) continue;

            if (checkRect.intersects(other->getGlobalBounds()))
            {
                //Colliding in space so cant spawn 
                destroyed = true;
                return;
            }
        }
    }

    //Spawn rope object
    Rope* obj = new Rope(ropeObject);

    obj->position = roundedPosition;
    //Chunk data
    currentChunk->objects.push_back(obj);
    if (obj->hasCollision)
        currentChunk->collisionObjects.push_back(obj);

    //Queue for removal
    destroyed = true;
};

void RopeProjectile::CalculatePhysics(std::vector<chunk*> chunkList) {

    velocity.y += gravity * deltaTime.asSeconds();

    //Collision detection

    bool test = false;

    for (chunk* _chunk : chunkList)
    {
        if (_chunk->collisionObjects.size() == 0) continue;
        for (Gameobject* otherobject : _chunk->collisionObjects)
        {
            if (!otherobject->hasCollision) {
                continue;
            }

            sf::Sprite* other = &otherobject->sprite;
            if (&sprite == other) continue;

            if (sprite.getGlobalBounds().intersects(other->getGlobalBounds()))
            {


                sf::Sprite _sprite = sprite;
                _sprite.setPosition(position);
                sf::FloatRect spriteRect = _sprite.getGlobalBounds();
                sf::FloatRect otherRect = other->getGlobalBounds();

                //Use correct rect when colliding with player
                if (Player* playerobject = dynamic_cast<Player*>(otherobject))
                    continue;

                //Not actually relative but good aproximate since in almost all cases only 1 of the objects is moving
                sf::Vector2<float> relativeVelocity = velocity;

                sf::Vector2<float> normal(0, 0);

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


                //Find side by checking smallest distance between the sides                    
                float minDistance = INFINITY;
                int side = -1;


                float topDistance = math::difference(spriteRect.top, otherBottom); // Top
                if (topDistance < minDistance) {
                    minDistance = topDistance;
                    spawnRope(chunkList); // Spawn rope if colliding to a block above
                }
                float rightDistance = math::difference(spriteRight, otherRect.left); // Right
                if (rightDistance < minDistance) {
                    minDistance = rightDistance;
                }
                float bottomDistance = math::difference(spriteBottom, otherRect.top); // Bottom
                if (bottomDistance < minDistance) {
                    minDistance = bottomDistance;
                }
                float leftDistance = math::difference(spriteRect.left, otherRight); // Left
                if (leftDistance < minDistance) {
                    minDistance = leftDistance;
                }

                //Top side
                bool sideCollision = topinsideother && !bottominsideother && (leftinsideother || rightinsideother);
                if (sideCollision && topDistance == minDistance) {
                    side = 0;
                    normal = sf::Vector2<float>(0.f, 1.f);
                }

                //Right side
                sideCollision = rightinsideother && !leftinsideother && (topinsideother || bottominsideother);
                if (sideCollision && rightDistance == minDistance) {
                    side = 1;
                    normal = sf::Vector2<float>(1.f, 0.f);
                }
                //Bottom side
                sideCollision = bottominsideother && !topinsideother && (leftinsideother || rightinsideother);
                if (sideCollision && bottomDistance == minDistance) {
                    side = 2;
                    normal = sf::Vector2<float>(0, -1.f);
                }
                //Left side
                sideCollision = leftinsideother && !rightinsideother && (topinsideother || bottominsideother);
                if (sideCollision && leftDistance == minDistance) {
                    side = 3;
                    normal = sf::Vector2<float>(-1.f, 0.f);
                }

                //Move to closest side
                switch (side) {
                case 0: // Top
                    position.y = otherBottom;
                    break;
                case 1: // Right
                    position.x = otherRect.left - spriteRect.width;
                    break;
                case 2: // Bottom
                    position.y = otherRect.top - spriteRect.height;
                    break;
                case 3: // Left
                    position.x = otherRight;
                    break;
                }

                //Take impact damage
                if (impactDamage > 0) {
                    int damage = int(velocity.y * impactDamage * 0.005);
                    if (damage > 0)
                        TakeDamage(float(damage));
                }

                //Negate velocity / bounce
                float totalVelocity = -1 * (normal * relativeVelocity) * (1 + bounciness);

                //Apply velocity;
                bool normalNegative = normal.y < 0 || normal.x < 0;
                bool addNegative = totalVelocity < 0;
                if ((normalNegative && !addNegative) || (addNegative && !normalNegative))
                    velocity += normal * totalVelocity;

                if (normal != sf::Vector2<float>(0, 0)) {
                    velocity += -relativeVelocity * friction * deltaTime.asSeconds();
                    test = true;
                }
            }
        }
    }
    colliding = test;



    //Scale the velocity to deltaTime to get consistent velocity across framerates
    position += velocity * deltaTime.asSeconds();

};

