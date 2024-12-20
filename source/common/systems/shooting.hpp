#pragma once

#ifndef SHOOTING_HPP
#define SHOOTING_HPP

#include <glm/gtx/euler_angles.hpp>

#include "../application.hpp"
#include "components/bullet.hpp"
#include "components/weapon.hpp"
#include "ecs/world.hpp"
#include "bullets.hpp"

namespace our {
    class Shooting {
        Application *app = nullptr;
        const nlohmann::json *data = nullptr;

    public:
        void enter(Application *app, const nlohmann::json &data) {
            this->app = app;
            for (const auto &component: data) {
                if (auto name = component.value("name", ""); name == "Bullets") {
                    this->data = &component;
                    break;
                }
            }

        }

        void update(World *world, float deltaTime, bulletsSystem *bulletsSystem) {
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

            Entity* bulletEntity = nullptr;
            for (const auto entity: world->getEntities()) {
                if (auto *weaponComponent = entity->getComponent<Weapon>()) {
                    if (app->getMouse().justPressed(GLFW_MOUSE_BUTTON_LEFT)) {
                        bulletEntity = world->add();
                        bulletEntity->deserializeBullet(*data);
                        bulletsSystem->addBullet(bulletEntity);
                        weaponComponent->lastShootTime = std::chrono::system_clock::now();
                        weaponComponent->isShooting = true;

                        auto bulletComponent = bulletEntity->getComponent<Bullet>();
                        bulletComponent->shooterName = "Player";
                        bulletComponent->lastShootTime = std::chrono::system_clock::now();
                        bulletComponent->isShot = true;
                        glm::vec3 bulletToNozzel = {0.1f, 0.1f, 0.1f};

                        const auto MW = weapon->getOwner()->getLocalToWorldMatrix();
                        const auto eyedW4d = MW * glm::vec4({0, 0, 0, 1});
                        const auto centerW4d = MW * glm::vec4({0, 0, 1, 1});

                        const auto eyeW = glm::vec3(eyedW4d.x, eyedW4d.y, eyedW4d.z);
                        const auto centerW = glm::vec3(centerW4d.x, centerW4d.y, centerW4d.z);
                        glm::vec3 translation = {MW[3][0], MW[3][1], MW[3][2]};

                        auto combinedDirection = normalize(centerW - eyeW);
                        bulletComponent->direction = combinedDirection;
                        glm::vec3 projection = glm::vec3(bulletToNozzel.x * combinedDirection.x,
                                                         bulletToNozzel.y * combinedDirection.y,
                                                         bulletToNozzel.z * combinedDirection.z);
       
                        translation += projection;
                        auto playrot = player->localTransform.rotation;
                        bulletEntity->localTransform.rotation = glm::vec3(playrot.x - glm::radians(90.0f),playrot.y, 0);
                        bulletEntity->localTransform.position = translation;
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
                    break;
                }
                if (auto *bulletComponent = entity->getComponent<Bullet>()) {
                    if (bulletComponent->isShot) {
                        entity->localTransform.position += bulletComponent->direction * bulletComponent->speed *
                                deltaTime;

                        auto currentTime = std::chrono::system_clock::now();
                        if (std::chrono::duration<float>(currentTime - bulletComponent->lastShootTime).count() > 20.0f) {
                            bulletsSystem->removeBullet(entity);
                            world->markForRemoval(entity);
                        }
                    }
                }
            }
        }
    };
} // our

#endif //SHOOTING_HPP
