#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "application.hpp"
#include "components/health.hpp"
#include "ecs/world.hpp"

namespace our {
    class PlayerSystem {
        Application *app;

    public:
        void enter(Application *app) {
            this->app = app;
        }

        void update(World *world, float deltaTime) {
            for (const auto entity: world->getEntities()) {
                if (entity->name == "Player") {
                    if (const auto health = entity->getComponent<HealthComponent>()) {
                        if (std::chrono::duration<float>(std::chrono::system_clock::now() - health->timeDied).count() >
                            5.0f) {
                            app->changeState("menu");
                        }
                    }
                }
            }
        }
    };
}
#endif //PLAYER_HPP
