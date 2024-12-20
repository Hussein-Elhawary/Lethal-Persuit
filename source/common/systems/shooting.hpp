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
            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            Weapon *weapon = nullptr;
            for (const auto entity: world->getEntities()) {
                //printf("Entity: %s\n", entity->name.c_str());
                if (entity->getComponent<CameraComponent>() && entity->getComponent<FreeCameraControllerComponent>()) {
                    camera = entity->getComponent<CameraComponent>();
                    controller = entity->getComponent<FreeCameraControllerComponent>();
                } else if (entity->getComponent<Weapon>()) {
                    weapon = entity->getComponent<Weapon>();
                }
                if (camera && controller && weapon) break;
            }
            if (!(camera && controller && weapon)) return;
            Entity *player = camera->getOwner();

            bool isShooting = false;
            for (const auto entity: world->getEntities()) {
                if (auto *weaponComponent = entity->getComponent<Weapon>()) {
                    if (app->getMouse().justPressed(GLFW_MOUSE_BUTTON_LEFT)) {
                        weaponComponent->lastShootTime = std::chrono::system_clock::now();
                        weaponComponent->isShooting = true;
                    }

                    const auto currentTime = std::chrono::system_clock::now();
                    if (const auto timeSinceLastShoot = std::chrono::duration<float>(
                            currentTime - weaponComponent->lastShootTime).count();
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
                    if (app->getMouse().justPressed(GLFW_MOUSE_BUTTON_LEFT) && !bulletComponent->isShot && !isShooting) {
                        bulletComponent->lastShootTime = std::chrono::system_clock::now();
                        bulletComponent->isShot = true;
                        isShooting = true;

                        const auto MW = weapon->getOwner()->getLocalToWorldMatrix();
                        const auto eyedW4d = MW * glm::vec4({0, 0, 0, 1});
                        const auto centerW4d = MW * glm::vec4({0, 0, 1, 1});

                        const auto eyeW = glm::vec3(eyedW4d.x, eyedW4d.y, eyedW4d.z);
                        const auto centerW = glm::vec3(centerW4d.x, centerW4d.y, centerW4d.z);
                        const glm::vec3 translation = {MW[3][0], MW[3][1], MW[3][2]};

                        auto combinedDirection = normalize(centerW - eyeW);
                        bulletComponent->direction = combinedDirection;

                        auto playrot = player->localTransform.rotation;
                        entity->localTransform.rotation = glm::vec3(playrot.x - glm::radians(90.0f),playrot.y, 0);
                        entity->localTransform.position = translation;
                    }

                    if (bulletComponent->isShot) {
                        entity->localTransform.position += bulletComponent->direction * bulletComponent->speed *
                                deltaTime;
                    }
                }
            }
        }
    };
} // our

#endif //SHOOTING_HPP
