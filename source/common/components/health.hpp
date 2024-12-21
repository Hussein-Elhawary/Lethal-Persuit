#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {

    class HealthComponent : public Component {
        
        public:
        int health = 1;
        bool status = true;         // true: Live,  false: Dead

        // The ID of this component type is "Health"
        static std::string getID() { return "Health"; }

        void deserialize(const nlohmann::json& data) override;


    };



}