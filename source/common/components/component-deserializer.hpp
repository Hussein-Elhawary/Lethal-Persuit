#pragma once

#include "bullet.hpp"
#include "../ecs/entity.hpp"
#include "camera.hpp"
#include "mesh-renderer.hpp"
#include "free-camera-controller.hpp"
#include "movement.hpp"
#include "collision.hpp"
#include "light.hpp"
#include "weapon.hpp"
#include "health.hpp"

namespace our {
    // Given a json object, this function picks and creates a component in the given entity
    // based on the "type" specified in the json object which is later deserialized from the rest of the json object
    inline void deserializeComponent(const nlohmann::json &data, Entity *entity) {
        const std::string type = data.value("type", "");
        const int n = data.value("n", 1);
        Component *component = nullptr;
        // TODO: (Req 8) Add an option to deserialize a "MeshRendererComponent" to the following if-else statement
        if (type == CameraComponent::getID()) {
            component = entity->addComponent<CameraComponent>();
        } else if (type == FreeCameraControllerComponent::getID()) {
            component = entity->addComponent<FreeCameraControllerComponent>();
        } else if (type == MovementComponent::getID()) {
            component = entity->addComponent<MovementComponent>();
        } else if (type == MeshRendererComponent::getID()) {
            component = entity->addComponent<MeshRendererComponent>();
        } else if (type == CollisionComponent::getID()) {
            component = entity->addComponent<CollisionComponent>();
        } else if (type == LightComponent::getID()) {
            component = entity->addComponent<LightComponent>();
        } else if (type == Weapon::getID()) {
            component = entity->addComponent<Weapon>();
        } else if (type == Bullet::getID()) {
            component = entity->addComponent<Bullet>();
        } else if (type == HealthComponent::getID()) {
            component = entity->addComponent<HealthComponent>();
        }
        if (component)
            component->deserialize(data);
        // if (type == CollisionComponent::getID())
        // {
        //     printf("CollisionComponent added\n");
        //     printf("box: x = %f, y = %f, z = %f\n", entity->getComponent<CollisionComponent>()->boundingBoxSize.x, entity->getComponent<CollisionComponent>()->boundingBoxSize.y, entity->getComponent<CollisionComponent>()->boundingBoxSize.z);
        // }
    }
}
