#pragma once

#ifndef WEAPON_HPP
#define WEAPON_HPP
#include <chrono>
#include <glm/vec3.hpp>

#include "ecs/component.hpp"

namespace our {
    class Weapon : public Component {
    public:
        std::chrono::time_point<std::chrono::system_clock> lastShootTime = std::chrono::system_clock::now();
        glm::vec3 initialPosition = glm::vec3(0, 0, 0);
        glm::vec3 maxPosition = glm::vec3(0, 0, 0);
        bool isShooting = false;
        std::string ownerName;

        static std::string getID() { return "Weapon"; }
        void deserialize(const nlohmann::json &) override;
    };
} // our

#endif //WEAPON_HPP
