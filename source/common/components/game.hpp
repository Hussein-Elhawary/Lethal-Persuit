#ifndef GAME_HPP
#define GAME_HPP
#include <chrono>

namespace our {
    class GameState {
    public:
        static std::chrono::time_point<std::chrono::system_clock> time;
        static bool died;
        static int numberOfEnemies;
    };
} // our

#endif //GAME_HPP
