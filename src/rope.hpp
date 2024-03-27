#ifndef ROPE_H
#define ROPE_H

#include "gameobject.hpp"

class Rope : public Gameobject {
	public:
		Rope();
		Rope(int length, float growDelay, std::vector<sf::Texture*> middleTexture, Gameobject baseObject, bool isStart = false, std::vector<sf::Texture*> startTexture = {nullptr});
		int length; // Amount of length of rope underneath it
		float growDelay; // Delay before trying to grow the rope
		bool hasGrown = false; // Has already tried to grow
		void OnLoop(std::vector<chunk*> chunkList);

	private:
		float delay;
		bool isStart;
		std::vector<sf::Texture*> middleTexture; // Texture for a middle piece
		std::vector<sf::Texture*> startTexture;
};

class RopeProjectile : public Gameobject {
	public:
		RopeProjectile();
		RopeProjectile(Rope ropeObject, Gameobject projectileObject);

	private:
		Rope ropeObject;
		void CalculatePhysics(std::vector<chunk*> chunkList);
		void spawnRope(std::vector<chunk*> chunkList);
};

#endif