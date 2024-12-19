#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/light.hpp"
#include "../asset-loader.hpp"

#include <glad/gl.h>
#include <vector>
#include <algorithm>
namespace our{
    class LightSystem {
    private:
        ShaderProgram* lightShader;
        void extractLightShaderFromAssets();
        void addLightComponentIfExist(Entity* entity,std::vector<LightComponent*>& lights);
        void sendLightComponentDataToLightShaders(const std::vector<LightComponent*>& lights);
        void sendCameraPositionToLightShaders(const glm::vec3& cameraPosition);
        void sendViewMatrixToLightShaders(const glm::mat4& viewMatrix);
        void sendLightAmbientComponentToLightShaders(const std::vector<LightComponent*>& lights);
        glm::vec3 calculateCameraPositionInWorld(const CameraComponent* camera);
        glm::ivec2 windowSize;
    public:
        void initialize(const glm::ivec2 windowSize);
        void addLightData(World* world);

    };
}




