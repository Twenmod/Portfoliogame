#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.hpp"

    class Enemy : public Gameobject{
        public:
            Enemy(float health, float speed, float attackDamage, Gameobject enemyObject);
            float health;
            float speed;
            float attackDamage;

            void CalculatePhysics(std::vector<chunk*> chunkList);

        private:
            bool moveDirection; // 1 is right 0 is left;
    };


#endif