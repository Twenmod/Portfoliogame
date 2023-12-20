#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.hpp"

    class Enemy : public Gameobject{
        public:
            Enemy(float health, float speed, Gameobject enemyObject);
            float health;
            float speed;

            void CalculatePhysics(std::vector<sf::Sprite*> collisionList);

        private:
            bool moveDirection; // 1 is right 0 is left;
    };


#endif