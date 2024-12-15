#include "../ecs/component.hpp"
#include "deserialize-utils.hpp"
#include "glm/gtx/string_cast.hpp"
#include "light.hpp"
#include <glm/glm.hpp>

namespace our {
    LightType LightComponent::parseType(std::string type) {
        if(type == "directional"){
            return LightType::DIRECTIONAL;
        }else if(type == "point"){
            return LightType::POINT;
        }else if(type == "spot"){
            return LightType::SPOT;
        }
        return LightType::DIRECTIONAL;
    }

    void LightComponent::deserializeAttenuation(const nlohmann::json &data) {
        auto attenuationIt = data.find("attenuation");
        if(attenuationIt != data.end()){
            attenuation.constant = attenuationIt->value("constant",0.0f);
            attenuation.linear = attenuationIt->value("constant",1.0f);
            attenuation.quadratic = attenuationIt->value("quadratic",0.0f);

        }
    }

    void LightComponent::deserializeAngle(const nlohmann::json &data) {
        auto spotAngleIt = data.find("angle");
        if(spotAngleIt != data.end()){
            spotAngle.inner = spotAngleIt->value("inner",0.0f);
            spotAngle.outer = spotAngleIt->value("outer",0.0f);
        }
    }

    void LightComponent::deserializePointLight(const nlohmann::json &data) {
        deserializeAttenuation(data);
    }

    void LightComponent::deserializeSpotLight(const nlohmann::json &data) {
        deserializeAttenuation(data);
        deserializeAngle(data);
    }

    void LightComponent::deserializeCommonAttr(const nlohmann::json &data) {
        std::string typeStr = data.value("lightType","directional");
        type = parseType(typeStr);
        diffuse = data.value("diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
        specular = data.value("specular", glm::vec3(0.0f, 0.0f, 0.0f));
        ambient = data.value("ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        direction = data.value("direction", glm::vec3(1.0f, 0.0f, 0.0f));
    }

    void LightComponent::deserialize(const nlohmann::json &data) {
        if (!data.is_object()) return;

        deserializeCommonAttr(data);

        if(type == LightType::POINT){
            deserializePointLight(data);
        }else if(type == LightType::SPOT){
            deserializeSpotLight(data);
        }

        printf("Diffuse %s Specular %s Ambient %s\n",glm::to_string(diffuse).c_str(),glm::to_string(specular).c_str(),glm::to_string(ambient).c_str());
        printf("Atten %f %f %f\n",attenuation.constant,attenuation.linear,attenuation.quadratic);
        printf("Angle %f %f \n",spotAngle.inner,spotAngle.outer);

    }
}