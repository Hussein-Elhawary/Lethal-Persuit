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
        int count = lights.size();
        lightShader->set("light_count", count);
        for (int i = 0;i<count;i++){
            auto const light = lights[i];
            std::string lightIdx = std::to_string(i);
            std::string lightPrefix = "lights["+lightIdx+"].";
            lightShader->set(lightPrefix+"type", static_cast<int>(light->type));
            
            lightShader->set(lightPrefix+"color", light->color);

            lightShader->set(lightPrefix+"position", light->position);
            lightShader->set(lightPrefix+"direction", light->direction);

            lightShader->set(lightPrefix+"ambient", light->ambient);
            lightShader->set(lightPrefix+"diffuse", light->diffuse);
            lightShader->set(lightPrefix+"specular", light->specular);


            lightShader->set(lightPrefix+"attenuation_constant", light->attenuation.constant);
            lightShader->set(lightPrefix+"attenuation_linear", light->attenuation.linear);
            lightShader->set(lightPrefix+"attenuation_quadratic", light->attenuation.quadratic);
            //convert angles to radian
            lightShader->set(lightPrefix+"inner_angle", light->spotAngle.inner*glm::pi<float>()/180);
            lightShader->set(lightPrefix+"outer_angle", light->spotAngle.outer*glm::pi<float>()/180);
        }
        sendLightAmbientComponentToLightShaders(lights);
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

    void LightSystem::sendLightAmbientComponentToLightShaders(const std::vector<LightComponent*>& lights) {
        //Average all ambient components to generate a single one and send to shader
        glm::vec3 ambient_component = glm::vec3(0.0f, 0.0f, 0.0f);
        for (const auto light: lights) {
            ambient_component += light->ambient;
        }
        ambient_component/=lights.size();
        lightShader->set("ambient_light", ambient_component);
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