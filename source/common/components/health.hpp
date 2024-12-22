#pragma once

#include <chrono>

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

#include "game.hpp"

namespace our {
    class HealthComponent : public Component {
    public:
        int health = 1;
        static int numberOfEntities;
        bool status = true; // true: Live,  false: Dead

        HealthComponent() { GameState::numberOfEnemies++; }
        // The ID of this component type is "Health"
        static std::string getID() { return "Health"; }

        void deserialize(const nlohmann::json &data) override;
    };
}
