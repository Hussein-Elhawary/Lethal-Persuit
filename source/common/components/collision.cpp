#include "collision.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    void CollisionComponent::deserialize(const nlohmann::json& data)
    {
        if (!data.is_object()) return;

        // Deserialize the collision type
        boundingBoxCenter = data.value("center", boundingBoxCenter);
        boundingBoxSize = data.value("size", boundingBoxSize);
    }

    void CollisionComponent::updateBoundingBoxToWorld(glm::mat4 modelMat, glm::vec3 scale)
    {
        glm::vec3 worldCenter = glm::vec3(modelMat * glm::vec4(boundingBoxCenter, 1.0f));
        //glm::vec3 worldSize = glm::vec3(modelMat * glm::vec4(boundingBoxSize, 0.0f));

        boundingBoxWorldCenter = worldCenter;
        //boundingBoxWorldSize = worldSize;
        boundingBoxWorldSize = scale * boundingBoxSize;
    }

    bool CollisionComponent::checkForCollision(CollisionComponent other)
    {
        glm::vec3 thisMin = boundingBoxWorldCenter - boundingBoxWorldSize;
        glm::vec3 thisMax = boundingBoxWorldCenter + boundingBoxWorldSize;

        glm::vec3 otherMin = other.boundingBoxWorldCenter - other.boundingBoxWorldSize;
        glm::vec3 otherMax = other.boundingBoxWorldCenter + other.boundingBoxWorldSize;

        return (thisMin.x <= otherMax.x && thisMax.x >= otherMin.x) &&
               (thisMin.y <= otherMax.y && thisMax.y >= otherMin.y) &&
               (thisMin.z <= otherMax.z && thisMax.z >= otherMin.z);
    }

}