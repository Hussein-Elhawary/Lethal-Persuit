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

    public:
        void enter(Application *app)
        {
            this->app = app;
            printf("Collision System Entered\n");
        }

        void update(World *world, float deltaTime)
        {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break

            std::vector <Entity *> collidingEntities;
            for (auto entity : world->getEntities())
            {
                // printf("Entity: %s\n", entity->name.c_str());
                // glm::vec3 position = entity->localTransform.position;
                // printf("Position: x = %f, y = %f, z = %f\n", position.x, position.y, position.z);
                
                if (auto collisionComponent = entity->getComponent<CollisionComponent>())
                {
                    printf("Component: Collision\n");
                    printf("Entity: %s\n", entity->name.c_str());
                    collisionComponent->updateBoundingBoxToWorld(entity->getLocalToWorldMatrix(), entity->localTransform.scale);
                    glm::vec3 box = collisionComponent->boundingBoxWorldSize;
                    // printf("Bounding Box: x = %f, y = %f, z = %f\n", box.x, box.y, box.z);
                    glm::vec3 boxCenter = collisionComponent->boundingBoxWorldCenter;
                    //printf("Bounding Box Center: x = %f, y = %f, z = %f with size x = %f, y = %f, z = %f\n", boxCenter.x, boxCenter.y, boxCenter.z, box.x, box.y, box.z);

                    collidingEntities.push_back(entity);
                }
            }

            for(auto entity1: collidingEntities)
            {
                for(auto entity2: collidingEntities)
                {
                    if(entity1 == entity2) continue;
                    if(auto collisionComponent1 = entity1->getComponent<CollisionComponent>())
                    {
                        if(auto collisionComponent2 = entity2->getComponent<CollisionComponent>())
                        {
                            if(collisionComponent1->checkForCollision(*collisionComponent2))
                            {
                                printf("Collision Detected\n");
                                MovementComponent* movement1 = entity1->getComponent<MovementComponent>();
                                //movement1->linearVelocity = glm::vec3(0, 0, 0);
                            }
                        }
                    }
                }
            }
        }
    };

}