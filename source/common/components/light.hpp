#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {
    enum class LightType{
        DIRECTIONAL,
        POINT,
        SPOT
    };
    class LightComponent : public Component {
    public:
        LightType type;
        // We also define the color & intensity of the light for each component of the Phong model (Ambient, Diffuse, Specular).
        glm::vec3 diffuse, specular, ambient;
        glm::vec3 position; // Used for Point and Spot Lights only
        glm::vec3 direction; // Used for Directional and Spot Lights only
        struct {
            float constant, linear, quadratic;
        } attenuation; // Used for Point and Spot Lights only

        // This specifies the inner and outer cone of the spot light.
        // The light power is 0 outside the outer cone, the light power is full inside the inner cone.
        // The light power is interpolated in between the inner and outer cone.
        struct {
            float inner, outer;
        } spotAngle; // Used for Spot Lights only
        static std::string getID() { return "Light"; }
        void deserialize(const nlohmann::json& data) override;
    private:
        LightType parseType(std::string type);
        void deserializeAttenuation(const nlohmann::json& data);
        void deserializeAngle(const nlohmann::json& data);
        void deserializeCommonAttr(const nlohmann::json& data);
        void deserializeSpotLight(const nlohmann::json& data);
        void deserializePointLight(const nlohmann::json& data);
    };

}