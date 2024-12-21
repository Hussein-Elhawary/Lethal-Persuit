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
            if (GameState::numberOfEnemies == 0 && std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - GameState::time).count() > 5) {
                app->changeState("menu");
                GameState::numberOfEnemies = -1;
            }
            else if (GameState::died && std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - GameState::time).count() > 5) {
                app->changeState("menu");
                GameState::died = false;
                GameState::numberOfEnemies = -1;
            }
        }
    };
}
#endif //PLAYER_HPP
