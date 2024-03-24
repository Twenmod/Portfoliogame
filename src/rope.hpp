#ifndef ROPE_H
#define ROPE_H

#include "gameobject.hpp"

class Rope : public Gameobject {
	public:
		Rope();
		Rope(int length, float growDelay, sf::Texture* middleTexture, Gameobject baseObject);
		int length; // Amount of length of rope underneath it
		float growDelay; // Delay before trying to grow the rope
		bool hasGrown = false; // Has already tried to grow
		void OnLoop(std::vector<chunk*> chunkList);

	private:
		float delay;
		sf::Texture* middleTexture; // Texture for a middle piece
};

#endif