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

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem {
        Application* app; // The application in which the state runs
        bool mouse_locked = true; // Is the mouse locked
        CollisionSystem collisionSystem;

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
            app->getMouse().lockMouse(app->getWindow());
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent 
        void update(World* world, float deltaTime) {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent* camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            for(auto entity : world->getEntities()){
                //printf("Entity: %s\n", entity->name.c_str());
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if(camera && controller) break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if(!(camera && controller)) return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity* entity = camera->getOwner();
            //printf("Entity: %s\n", entity->name.c_str());
            collisionSystem.setPlayer(entity);
            // printf("before ms_clicked\n");
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_2)) 
            {
                // printf("ms_clicked\n");
                // if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked)
                // {
                //     app->getMouse().lockMouse(app->getWindow());
                //     mouse_locked = true;
                //     // If the left mouse button is released, we unlock and unhide the mouse.
                // }
                // else if (!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked)
                // {
                //     app->getMouse().unlockMouse(app->getWindow());
                //     mouse_locked = false;
                // }

                // We get a reference to the entity's position and rotation
                glm::vec3 &position = entity->localTransform.position;
                glm::vec3 &rotation = entity->localTransform.rotation;

                // If the left mouse button is pressed, we get the change in the mouse location
                // and use it to update the camera rotation
                //if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1))
                //{
                glm::vec2 delta = app->getMouse().getMouseDelta();
                rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
                rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
                //}

                // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
                if (rotation.x < -glm::half_pi<float>() * 0.99f)
                    rotation.x = -glm::half_pi<float>() * 0.99f;
                if (rotation.x > glm::half_pi<float>() * 0.99f)
                    rotation.x = glm::half_pi<float>() * 0.99f;
                // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
                // This could prevent floating point error if the player rotates in single direction for an extremely long time.
                rotation.y = glm::wrapAngle(rotation.y);

                // We update the camera fov based on the mouse wheel scrolling amount
                float fov = camera->fovY + app->getMouse().getScrollOffset().y * controller->fovSensitivity;
                fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
                camera->fovY = fov;

                // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
                glm::mat4 matrix = entity->localTransform.toMat4();

                glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                          up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                          right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

                glm::vec3 current_sensitivity = controller->positionSensitivity;
                // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
                if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
                    current_sensitivity *= controller->speedupFactor;

                // We change the camera position based on the keys WASD/QE
                // S & W moves the player back and forth
                if (app->getKeyboard().isPressed(GLFW_KEY_W))
                    position += front * (deltaTime * current_sensitivity.z*4);
                if (app->getKeyboard().isPressed(GLFW_KEY_S))
                    position -= front * (deltaTime * current_sensitivity.z*4);
                // Q & E moves the player up and down
                if (app->getKeyboard().isPressed(GLFW_KEY_Q))
                    position += up * (deltaTime * current_sensitivity.y*4);
                if (app->getKeyboard().isPressed(GLFW_KEY_E))
                    position -= up * (deltaTime * current_sensitivity.y*4);
                // A & D moves the player left or right
                if (app->getKeyboard().isPressed(GLFW_KEY_D))
                    position += right * (deltaTime * current_sensitivity.x*4);
                if (app->getKeyboard().isPressed(GLFW_KEY_A))
                    position -= right * (deltaTime * current_sensitivity.x*4);
                return;
            }
            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            //if(app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked){
            //    app->getMouse().lockMouse(app->getWindow());
            //    mouse_locked = true;
            //// If the left mouse button is released, we unlock and unhide the mouse.
            //} else if(!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked) {
            //    app->getMouse().unlockMouse(app->getWindow());
            //    mouse_locked = false;
            //}

            // We get a reference to the entity's position and rotation
            glm::vec3& position = entity->localTransform.position;
            glm::vec3& rotation = entity->localTransform.rotation;

            // If the left mouse button is pressed, we get the change in the mouse location
            // and use it to update the camera rotation
            //if(app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1)){
            glm::vec2 delta = app->getMouse().getMouseDelta();
            rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
            rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
            //}

            // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
            if(rotation.x < -glm::half_pi<float>() * 0.99f) rotation.x = -glm::half_pi<float>() * 0.99f;
            if(rotation.x >  glm::half_pi<float>() * 0.99f) rotation.x  = glm::half_pi<float>() * 0.99f;
            // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
            // This could prevent floating point error if the player rotates in single direction for an extremely long time. 
            rotation.y = glm::wrapAngle(rotation.y);

            // We update the camera fov based on the mouse wheel scrolling amount
            float fov = camera->fovY + app->getMouse().getScrollOffset().y * controller->fovSensitivity;
            fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
            camera->fovY = fov;

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)), 
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;
            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            if(app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT)) current_sensitivity *= controller->speedupFactor;

            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
                    
            glm::vec3 old_position = position;
            if (controller->inDash)
            {
                position += (controller->dashDirection * (deltaTime * 100.0f))*glm::vec3(1, 0, 1);
                if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - controller->lastDashTime).count() > 200)
                {
                    controller->inDash = false;
                    fov = camera->fovY - controller->dashfov;

                }
                fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
                camera->fovY = fov;
            }
            
            if(app->getKeyboard().isPressed(GLFW_KEY_W)) position += front * (deltaTime * current_sensitivity.z)*glm::vec3(1, 0, 1);
            if(app->getKeyboard().isPressed(GLFW_KEY_S)) position -= front * (deltaTime * current_sensitivity.z)*glm::vec3(1, 0, 1);
            // Q & E moves the player up and down
            //if(app->getKeyboard().isPressed(GLFW_KEY_Q)) position += up * (deltaTime * current_sensitivity.y);
            //if(app->getKeyboard().isPressed(GLFW_KEY_E)) position -= up * (deltaTime * current_sensitivity.y);
            // A & D moves the player left or right 
            if(app->getKeyboard().isPressed(GLFW_KEY_D)) position += right * (deltaTime * current_sensitivity.x);
            if(app->getKeyboard().isPressed(GLFW_KEY_A)) position -= right * (deltaTime * current_sensitivity.x);
            // Tp ability
            if(app->getKeyboard().justPressed(GLFW_KEY_Q))
            {
                if(!controller->tpSet)
                {
                    controller->tp = position;
                    controller->tpSet = true;
                }
                else
                {
                    position = controller->tp;
                    controller->tpSet = false;
                }
            }
            
            // Dash ability
            if(app->getKeyboard().justPressed(GLFW_KEY_F))
            {
                if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - controller->lastDashTime).count() > controller->dashCoolDown)
                {
                    controller->lastDashTime = std::chrono::system_clock::now();
                    controller->inDash = true;
                    controller->dashDirection = front;
                    fov = camera->fovY + controller->dashfov;
                    fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
                    camera->fovY = fov;
                }
            }
            // // jump ability TODO: change this to colision with the ground
            bool isCollidedWithGround = collisionSystem.playerGroundCollision(world, deltaTime, position);
            //fprintf(stderr, "Collision detected with ground: %s\n", isCollidedWithGround ? "true" : "false");
            //printf("jump pressed: %s\n", app->getKeyboard().justPressed(GLFW_KEY_SPACE) ? "true" : "false"); 
            bool jumpInCurrentFrame = false;
            if(isCollidedWithGround && app->getKeyboard().justPressed(GLFW_KEY_SPACE) )
            {
                controller->upSpeed = controller->jumpSpeed;
                //printf("Jumping\n");
                jumpInCurrentFrame = true;
            }

            // TODO: change this to check if collision with the ground
            if (!isCollidedWithGround)
            {
                controller->upSpeed -= deltaTime * controller->gravity;
            }
              
            position.y += controller->upSpeed * deltaTime;
            if(isCollidedWithGround && !jumpInCurrentFrame )
            {
                controller->upSpeed = -0.1;
                position.y = old_position.y;
            }

            glm::vec3 direction = position - old_position;
            bool isCollide = collisionSystem.playerCollisionUpdate(world, deltaTime, position);
            Entity* collidedEntity = collisionSystem.playerCollisionEntity(world, deltaTime, position);
            //printf("Collision detected: %d\n", isCollide);
            if(isCollide){
                //wall jump
                if (collidedEntity->name.substr(0,4) == "Wall")
                {
                    if(app->getKeyboard().justPressed(GLFW_KEY_SPACE))
                    {
                        controller->upSpeed = controller->jumpSpeed;
                        position.y += controller->upSpeed * deltaTime;
                        position += direction * deltaTime;
                    }
                }
                
                //position.y = old_position.y;
                glm::vec3 possiblePosition = position;
                position = old_position + glm::vec3(0.1,0,0);
                bool isCollideXPos = collisionSystem.playerCollisionUpdate(world, deltaTime, position);
                position = old_position + glm::vec3(-0.1,0,0);
                bool isCollideXNeg = collisionSystem.playerCollisionUpdate(world, deltaTime, position);
                position = old_position + glm::vec3(0,0,0.1);
                bool isCollideZPos = collisionSystem.playerCollisionUpdate(world, deltaTime, position);
                position = old_position + glm::vec3(0,0,-0.1);
                bool isCollideZNeg = collisionSystem.playerCollisionUpdate(world, deltaTime, position);
                position = possiblePosition;
                if(direction.x != 0)
                {
                    if(isCollideXPos || isCollideXNeg)
                        position.x = old_position.x;
                }
                if(direction.z != 0)
                {
                    if(isCollideZPos || isCollideZNeg)
                        position.z = old_position.z;
                }
            }
            // if under the map reset the position
            if(position.y < -100)
            {
                position.y = 2;
                controller->upSpeed = 0;
            }
            // updates Y position
            printf("Camera Position: (%f, %f, %f)\n", position.x, position.y, position.z);
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit(){
            if(mouse_locked) {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }

    };

}
