#pragma once

#include <asset-loader.hpp>
#include <ecs/world.hpp>
#include <components/camera.hpp>
#include <components/mesh-renderer.hpp>
#include <application.hpp>

// This is a helper function that will search for a component and will return the first one found
template<typename T>
T *find(our::World *world) {
    for (auto &entity: world->getEntities()) {
        if (T *component = entity->getComponent<T>())
            return component;
    }
    return nullptr;
}

// This state tests and shows how to use the ECS framework and deserialization.
class EntityTestState final : public our::State {
    our::World world;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets")) {
            our::deserializeAllAssets(config["assets"]);
        }

        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world")) {
            world.deserialize(config["world"]);
        }
    }

    void onDraw(double deltaTime) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // First, we look for a camera and if none was found, we return (there is nothing we can render)
        const auto *camera = find<our::CameraComponent>(&world);
        if (camera == nullptr) return;

        // Then we compute the VP matrix from the camera
        const glm::ivec2 size = getApp()->getFrameBufferSize();
        //TODO: (Req 8) Change the following line to compute the correct view projection matrix 
        //first multiply by view matrix then by projection matrix
        const glm::mat4 VP = camera->getProjectionMatrix(size) * camera->getViewMatrix();

        for (auto &entity: world.getEntities()) {
            // For each entity, we look for a mesh renderer (if none was found, we skip this entity)
            const auto *meshRenderer = entity->getComponent<our::MeshRendererComponent>();
            if (meshRenderer == nullptr) continue;

            //TODO: (Req 8) Complete the loop body to draw the current entity
            // Then we setup the material, send the transform matrix to the shader then draw the mesh

            const glm::mat4 transformation = VP * entity->getLocalToWorldMatrix();
            meshRenderer->material->setup();
            meshRenderer->material->shader->set("transform", transformation);
            meshRenderer->mesh->draw();
        }
    }

    void onDestroy() override {
        world.clear();
        our::clearAllAssets();
    }
};
