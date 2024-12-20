#pragma once

#ifndef SHOOTING_HPP
#define SHOOTING_HPP

#include <glm/gtx/euler_angles.hpp>

#include "../application.hpp"
#include "components/bullet.hpp"
#include "components/weapon.hpp"
#include "ecs/world.hpp"

namespace our {
    class Shooting {
        Application *app = nullptr;

    public:
        void enter(Application *app) {
            this->app = app;
        }

        void update(World *world, float deltaTime) {
            CameraComponent* camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            for(const auto entity : world->getEntities()){
                //printf("Entity: %s\n", entity->name.c_str());
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if(camera && controller) break;
            }
            if(!(camera && controller)) return;
            Entity* player = camera->getOwner();

            for (const auto entity: world->getEntities()) {
                if (auto *weaponComponent = entity->getComponent<Weapon>()) {
                    if (app->getMouse().justPressed(GLFW_MOUSE_BUTTON_LEFT)) {
                        weaponComponent->lastShootTime = std::chrono::system_clock::now();
                        weaponComponent->isShooting = true;
                    }

                    const auto currentTime = std::chrono::system_clock::now();
                    if (const auto timeSinceLastShoot = std::chrono::duration<float>(currentTime - weaponComponent->lastShootTime).count();
                        timeSinceLastShoot < 0.1f && weaponComponent->isShooting) {
                        entity->localTransform.position =
                                weaponComponent->initialPosition + glm::vec3(0, 0, 1) * timeSinceLastShoot * 0.1f;
                        weaponComponent->maxPosition = entity->localTransform.position;
                    } else if (timeSinceLastShoot < 0.2f && weaponComponent->isShooting) {
                        entity->localTransform.position =
                                weaponComponent->maxPosition - glm::vec3(0, 0, 1) * (timeSinceLastShoot - 0.1f) * 0.1f;
                    }
                }
                if (auto *bulletComponent = entity->getComponent<Bullet>()) {
                    if (app->getMouse().justPressed(GLFW_MOUSE_BUTTON_LEFT)) {
                        bulletComponent->lastShootTime = std::chrono::system_clock::now();
                        bulletComponent->isShot = true;

                        entity->localTransform.position = player->localTransform.position;

                        const auto M = player->getLocalToWorldMatrix();
                        const glm::vec4 eye4d = M * glm::vec4({0, 0, 0, 1});
                        const glm::vec4 center4d = M * glm::vec4({0, 0, -1, 1});
                        const auto eye = glm::vec3(eye4d.x, eye4d.y, eye4d.z);
                        const auto center = glm::vec3(center4d.x, center4d.y, center4d.z);
                        bulletComponent->direction = center - eye;

                        entity->localTransform.rotation = glm::vec3(player->localTransform.rotation.x - glm::radians(90.0f),player->localTransform.rotation.y, 0);
                    }

                    if (bulletComponent->isShot) {
                        entity->localTransform.position += bulletComponent->direction * bulletComponent->speed * deltaTime;
                        printf("Bullet position: x = %f, y = %f, z = %f\n", entity->localTransform.position.x,
                               entity->localTransform.position.y, entity->localTransform.position.z);
                    }

                }
            }
        }
    };
} // our

#endif //SHOOTING_HPP
