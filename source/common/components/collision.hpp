#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {


    class CollisionComponent : public Component {
    public:
        glm::vec3 boundingBox = glm::vec3(0, 0, 0);
        // The ID of this component type is "Collision"
        CollisionComponent() {
            printf("CollisionComponent created\n");
            printf("box: x = %f, y = %f, z = %f\n", boundingBox.x, boundingBox.y, boundingBox.z);
        }

        static std::string getID() { return "Collision"; }

        void deserialize(const nlohmann::json& data) override;
    };

}