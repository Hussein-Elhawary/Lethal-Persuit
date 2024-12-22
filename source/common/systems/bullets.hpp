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
#include "components/health.hpp"

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

                        if (auto bulletCollisionComponent = currentBullet->getComponent<CollisionComponent>())
                        {
                            if (bulletComponent->shooterName == entity->name)
                                continue;
                            if (auto collisionComponent = entity->getComponent<CollisionComponent>())
                            {
                                if (bulletCollisionComponent->checkForCollision(*collisionComponent))
                                {
                                    // printf("Current Bullet: %s\n", currentBullet->name.c_str());
                                    // printf("Colliding Entity: %s\n", entity->name.c_str());
                                    removeBullet(currentBullet);
                                    world->markForRemoval(currentBullet);

                                    if (auto healthComponent = entity->getComponent<HealthComponent>())
                                    {
                                        if (healthComponent->status == false)
                                            continue;
                                        healthComponent->health -= 1;
                                        if (healthComponent->health <= 0)
                                        {
                                            if (entity->name == "Player")
                                            {
                                                GameState::died = true;
                                            }
                                            GameState::numberOfEnemies--;
                                            GameState::time = std::chrono::system_clock::now();

                                            healthComponent->status = false;
                                            entity->localTransform.rotation = glm::vec3(glm::radians(-90.0f), 0, 0);
                                            entity->localTransform.position.y += 0.2;
                                            collisionComponent->boundingBoxSize = glm::vec3(0, 0, 0); // Set the bounding box size to 0 to avoid collision
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        void clear()
        {
            // empty the bullets vector
            bulletsEntities.clear();
        }
    };
}