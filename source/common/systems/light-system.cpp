#include "light-system.hpp"
#include "glm/gtx/string_cast.hpp"
namespace our{

    void LightSystem::initialize(const glm::ivec2 windowSize) {
        extractLightShaderFromAssets();
        this->windowSize = windowSize;
    }

    void LightSystem::extractLightShaderFromAssets() {
        lightShader = AssetLoader<ShaderProgram>::get("lighted");
        if (!lightShader) {
            lightShader = new ShaderProgram();
            lightShader->attach("assets/shaders/light.vert", GL_VERTEX_SHADER);
            lightShader->attach("assets/shaders/light.frag", GL_FRAGMENT_SHADER);
            lightShader->link();
        }
    }

    void LightSystem::addLightComponentIfExist(Entity *entity, std::vector<LightComponent*>& lights) {
        if (const auto light = entity->getComponent<LightComponent>(); light) {
            lights.push_back(light);
        }
    }

    void LightSystem::sendLightComponentDataToLightShaders(const std::vector<LightComponent*>& lights) {
        for (const auto light: lights) {
            lightShader->set("light.position", light->position);
            lightShader->set("light.direction", light->direction);
            lightShader->set("light.ambient", light->ambient);
            lightShader->set("light.diffuse", light->diffuse);
            lightShader->set("light.specular", light->specular);

            lightShader->set("light.attenuation_constant", light->attenuation.constant);
            lightShader->set("light.attenuation_linear", light->attenuation.linear);
            lightShader->set("light.attenuation_quadratic", light->attenuation.quadratic);

        }
    }

    void LightSystem::sendCameraPositionToLightShaders(const glm::vec3 &cameraPosition) {
        lightShader->set("camera_position", cameraPosition);
    }

    glm::vec3 LightSystem::calculateCameraPositionInWorld(const our::CameraComponent *camera) {
        const auto owner = camera->getOwner();
        const auto M = owner->getLocalToWorldMatrix();
        const glm::vec4 eye4d = M * glm::vec4({0, 0, 0, 1});
        return glm::vec3(eye4d.x, eye4d.y, eye4d.z);
    }

    void LightSystem::sendViewMatrixToLightShaders(const glm::mat4 &viewMatrix) {
        lightShader->set("view_projection", viewMatrix);
    }

    void LightSystem::addLightData(our::World *world) {
        std::vector<LightComponent*> lights;
        const CameraComponent* camera = nullptr;
        for (const auto entity: world->getEntities()) {
            if(camera == nullptr){
                camera = entity->getComponent<CameraComponent>();
            }
            addLightComponentIfExist(entity, lights);
        }
        lightShader->use();
        sendLightComponentDataToLightShaders(lights);
        if(camera != nullptr){
            glm::vec3 cameraPosition = calculateCameraPositionInWorld(camera);
            sendCameraPositionToLightShaders(cameraPosition);

            glm::mat4 viewProjectionMatrix = camera->getProjectionMatrix(windowSize) * camera->getViewMatrix();
            sendViewMatrixToLightShaders(viewProjectionMatrix);
        }

    }

}