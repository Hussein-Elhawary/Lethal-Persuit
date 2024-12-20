#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/collision.hpp"
#include "../application.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{
    class CollisionSystem
    {
        Application *app; // The application in which the state runs
        Entity *player = nullptr;

    public:
        glm::vec3 old_position = glm::vec3(0, 0, 0);
        void enter(Application *app)
        {
            this->app = app;
        }

        void setPlayer(Entity *player)
        {
            this->player = player;
        }

        bool playerCollisionUpdate(World *world, float deltaTime, glm::vec3 old_position)
        {
            std::vector<Entity *> collidingEntities;
            for (auto entity : world->getEntities())
            {
                if (auto collisionComponent = entity->getComponent<CollisionComponent>())
                {
                    collisionComponent->updateBoundingBoxToWorld(entity->getLocalToWorldMatrix(), entity->localTransform.scale);
                    glm::vec3 box = collisionComponent->boundingBoxWorldSize;
                    glm::vec3 boxCenter = collisionComponent->boundingBoxWorldCenter;
                    if(entity->name == "Player") continue;
                    if(entity->name == "Ground")continue;
                    if(entity->name == "Bullets") continue;
                    
                    collidingEntities.push_back(entity);
                }
            }
            for (auto entity : collidingEntities)
            {
                if (auto collisionComponent1 = player->getComponent<CollisionComponent>())
                {
                    if (auto collisionComponent2 = entity->getComponent<CollisionComponent>())
                    {
                        if (collisionComponent1->checkForCollision(*collisionComponent2))
                        {
                            return true;
                        }
                    }
                }
            }

            return false;
        }

        Entity* playerCollisionEntity(World *world, float deltaTime, glm::vec3 old_position)
        {
            std::vector<Entity *> collidingEntities;
            for (auto entity : world->getEntities())
            {
                if (auto collisionComponent = entity->getComponent<CollisionComponent>())
                {
                    collisionComponent->updateBoundingBoxToWorld(entity->getLocalToWorldMatrix(), entity->localTransform.scale);
                    glm::vec3 box = collisionComponent->boundingBoxWorldSize;
                    glm::vec3 boxCenter = collisionComponent->boundingBoxWorldCenter;
                    if(entity->name == "Player") continue;
                    if(entity->name == "Ground") continue;
                    if(entity->name == "Bullets") continue;

                    
                    collidingEntities.push_back(entity);
                }
            }
            for (auto entity : collidingEntities)
            {
                if (auto collisionComponent1 = player->getComponent<CollisionComponent>())
                {
                    if (auto collisionComponent2 = entity->getComponent<CollisionComponent>())
                    {
                        if (collisionComponent1->checkForCollision(*collisionComponent2))
                        {
                            return entity;
                        }
                    }
                }
            }

            return nullptr;
        }

        bool playerGroundCollision(World *world, float deltaTime, glm::vec3 old_position)
        {
            std::vector<Entity *> collidingEntities;
            for (auto entity : world->getEntities())
            {
                if (auto collisionComponent = entity->getComponent<CollisionComponent>())
                {
                    collisionComponent->updateBoundingBoxToWorld(entity->getLocalToWorldMatrix(), entity->localTransform.scale);
                    if(entity->name == "Player") continue;
                    if(entity->name != "Ground")continue;

                    collidingEntities.push_back(entity);
                }
            }
            for (auto entity : collidingEntities)
            {
                if (auto collisionComponent1 = player->getComponent<CollisionComponent>())
                {

                    if (auto collisionComponent2 = entity->getComponent<CollisionComponent>())
                    {
                        if (collisionComponent1->checkForCollision(*collisionComponent2))
                        {
                            return true;
                        }
                    }
                }
            }

            return false;
        }

        std::vector<Entity *> getCollidingEntitiesWithoutBullets(World *world)
        {
            std::vector<Entity *> collidingEntities;
            for (auto entity : world->getEntities())
            {
                if (auto collisionComponent = entity->getComponent<CollisionComponent>())
                {
                    if(entity->name == "Bullets")continue;
                    collisionComponent->updateBoundingBoxToWorld(entity->getLocalToWorldMatrix(), entity->localTransform.scale);

                    collidingEntities.push_back(entity);
                }
            }
           
            return collidingEntities;
        }

    };

}