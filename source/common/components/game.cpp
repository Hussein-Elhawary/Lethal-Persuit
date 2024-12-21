#include "game.hpp"

namespace our {
    std::chrono::time_point<std::chrono::system_clock> GameState::time = std::chrono::system_clock::now();
    bool GameState::died = 0;
    int GameState::numberOfEnemies = -1;
}