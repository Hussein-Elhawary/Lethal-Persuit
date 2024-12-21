#pragma once

#include <chrono>

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {
    class HealthComponent : public Component {
    public:
        int health = 1;
        bool status = true; // true: Live,  false: Dead
        std::chrono::time_point<std::chrono::system_clock> timeDied = std::chrono::system_clock::now() + std::chrono::minutes(15);

        // The ID of this component type is "Health"
        static std::string getID() { return "Health"; }

        void deserialize(const nlohmann::json &data) override;
    };
}
