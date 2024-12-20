#pragma once

#ifndef BULLET_HPP
#define BULLET_HPP
#include <chrono>
#include <glm/vec3.hpp>

#include "ecs/component.hpp"

namespace our {
    class Bullet : public Component {
    public:
        glm::vec3 direction = {0, 0, 0};
        std::chrono::time_point<std::chrono::system_clock> lastShootTime = std::chrono::system_clock::now();
        bool isShot = false;
        float speed = 0;
        static std::string getID() { return "Bullet"; }
        void deserialize(const nlohmann::json &data) override;
    };
} // our

#endif //BULLET_HPP
