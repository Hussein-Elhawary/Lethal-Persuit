#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {


    class CollisionComponent : public Component {
    public:

        // collision by bounding box
        //glm::vec3 boundingBox = glm::vec3(0, 0, 0);
        glm::vec3 boundingBoxCenter = glm::vec3(0, 0, 0);
        glm::vec3 boundingBoxSize = glm::vec3(0, 0, 0);
        glm::vec3 boundingBoxWorldCenter = glm::vec3(0, 0, 0);
        glm::vec3 boundingBoxWorldSize = glm::vec3(0, 0, 0);

        //CollisionComponent() {}

        // The ID of this component type is "Collision"
        static std::string getID() { return "Collision"; }

        void deserialize(const nlohmann::json& data) override;

        void updateBoundingBoxToWorld(glm::mat4 modelMat, glm::vec3 scale);

        bool checkForCollision(CollisionComponent other);

    };

}