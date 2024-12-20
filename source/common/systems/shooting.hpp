#pragma once

#ifndef SHOOTING_HPP
#define SHOOTING_HPP

#include "../application.hpp"
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
            for (const auto entity: world->getEntities()) {
                if (auto *weaponComponent = entity->getComponent<Weapon>()) {
                    if (app->getMouse().justPressed(GLFW_MOUSE_BUTTON_LEFT)) {
                        weaponComponent->lastShootTime = std::chrono::system_clock::now();
                        weaponComponent->isShooting = true;
                    }

                    const auto currentTime = std::chrono::system_clock::now();
                    if (const auto timeSinceLastShoot = std::chrono::duration<float>(currentTime - weaponComponent->lastShootTime).count(); timeSinceLastShoot < 0.1f && weaponComponent->isShooting) {
                        entity->localTransform.position = weaponComponent->initialPosition + glm::vec3(0, 0, 1) * timeSinceLastShoot * 0.1f;
                        weaponComponent->maxPosition = entity->localTransform.position;
                    }
                    else if (timeSinceLastShoot < 0.2f && weaponComponent->isShooting) {
                        entity->localTransform.position = weaponComponent->maxPosition - glm::vec3(0, 0, 1) * (timeSinceLastShoot - 0.1f) * 0.1f;
                    }
                }
            }
        }
    };
} // our

#endif //SHOOTING_HPP
