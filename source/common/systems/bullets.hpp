#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/collision.hpp"
#include "../application.hpp"
#include "collision.hpp"
#include "../components/mesh-renderer.hpp"
#include "bullets.hpp"
#include "components/bullet.hpp"

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
        }

        void removeBullet(Entity *bullet)
        {
            auto it = std::remove(bulletsEntities.begin(), bulletsEntities.end(), bullet);
            if (it != bulletsEntities.end())
            {
                bulletsEntities.erase(it, bulletsEntities.end());
            }
        }

        void update(World *world, float deltaTime)
        {
            std::vector<Entity *> collidingEntities;

            collidingEntities = collisionSystem.getCollidingEntitiesWithoutBullets(world);
            
            for (auto currentBullet : bulletsEntities)
            {
                if (auto bulletComponent = currentBullet->getComponent<Bullet>())
                {
                    for (auto entity : collidingEntities)
                    {
                        if (auto collisionComponent1 = currentBullet->getComponent<CollisionComponent>())
                        {
                            if (bulletComponent->shooterName == entity->name)
                                continue;
                            if (auto collisionComponent2 = entity->getComponent<CollisionComponent>())
                            {
                                if (collisionComponent1->checkForCollision(*collisionComponent2))
                                {
                                    removeBullet(currentBullet);
                                    world->markForRemoval(currentBullet);
                                }
                            }
                        }
                    }
                }
            }
        };
    };
}