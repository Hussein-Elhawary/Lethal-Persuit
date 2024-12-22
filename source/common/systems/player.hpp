#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "application.hpp"
#include "components/health.hpp"
#include "ecs/world.hpp"

namespace our {
    class PlayerSystem {
        Application *app = nullptr;

    public:
        void enter(Application *app) {
            this->app = app;
        }

        void update(World *world, float deltaTime) {
            if (GameState::numberOfEnemies == 0 && std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - GameState::time).count() > 2) {
                app->changeState("win");
                GameState::numberOfEnemies = -1;
            }
            else if (GameState::died && std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - GameState::time).count() > 2) {
                app->changeState("lose");
                GameState::died = false;
                GameState::numberOfEnemies = -1;
            }
        }
    };
}
#endif //PLAYER_HPP
