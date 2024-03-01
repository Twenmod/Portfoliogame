#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.hpp"

    class Enemy : public Gameobject{
        public:
            Enemy(Gameobject enemyObject, float health = 10, float speed = 5, float attackDamage = 1, float attackInterval = 1);
            float health;
            float speed;
            float attackDamage;
            float attackInterval;

            void CalculatePhysics(std::vector<chunk*> chunkList);
            void OnLoop(std::vector<chunk*> chunkList);
            void OnRender();
            void TakeDamage(float damage);
    private:
            bool moveDirection; // 1 is right 0 is left;
            float attackTimer = 0;
            float damageAnimation = 0;
    };


#endif