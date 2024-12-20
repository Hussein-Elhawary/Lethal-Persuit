#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/collision.hpp"
#include "../application.hpp"
#include "collision.hpp"
#include "../components/mesh-renderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    class bulletsSystem
    {
        Application *app;
        std::vector<Entity *> bulletsEntities;
        CollisionSystem collisionSystem;

    public:
        void enter(Application *app)
        {
            this->app = app;
        }

        void addBullet(Entity *bullet)
        {
            bulletsEntities.push_back(bullet);
            printf("Bullet Added: %s\n", bullet->name.c_str());
        }

        // need to be fixed
        bool removeBullet(Entity *bullet)
        {
            auto it = std::remove(bulletsEntities.begin(), bulletsEntities.end(), bullet);
            if (it != bulletsEntities.end())
            {
                bulletsEntities.erase(it, bulletsEntities.end());
                printf("bulletsEntities Size: %d\n", bulletsEntities.size());
                return true;
            }
            return false;
        }

        void update(World *world, float deltaTime)
        {
            std::vector<Entity*> collidingEntities;
            collidingEntities = collisionSystem.getCollidingEntitiesWithoutBullets(world);
            // printf("Entered Bullet Update\n");
            //printf("Colliding Entities Size: %d\n", collidingEntities.size());
            int i = 1;
            //printf("bulletsEntites Size: %d\n", bulletsEntities.size());
            for (auto currentBullet : bulletsEntities)
            {   
                printf("Current Bullet %d: %s\n",i, currentBullet->name.c_str());
                collisionSystem.bulletCollision(world, deltaTime, currentBullet, collidingEntities);
                i++;
            }
        };
    };
}