#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp> 
#include <chrono>

namespace our {

    // This component denotes that the FreeCameraControllerSystem will move the owning entity using user inputs.
    // It will also control the camera field of view accroding to the scrolling of the mouse wheel
    // This component is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/systems/free-camera-controller.hpp"
    // For a more simple example of how to use the ECS framework, see "movement.hpp"
    class FreeCameraControllerComponent : public Component {
    public:
        // The senstivity paramter defined sensitive the camera rotation & fov is to the mouse moves and wheel scrolling
        float rotationSensitivity = 0.002f; // The angle change per pixel of mouse movement
        float fovSensitivity = 0.3f; // The fov angle change per unit of mouse wheel scrolling
        glm::vec3 positionSensitivity = {6.0f, 6.0f, 6.0f}; // The unity per second of camera movement if WASD is pressed
        glm::vec3 originalSpeed = {6.0f, 6.0f, 6.0f}; // The original speed of the camera
        float speedupFactor = 0.50f; // A multiplier for the positionSensitivity if "Left Shift" is held.
        bool tpSet = false; //check if tp is set
        glm::vec3 tp = glm::vec3(0, 0, 0); //tp position
        int dashCoolDown = 0; //dash cool down
        std::chrono::time_point<std::chrono::system_clock> lastDashTime = std::chrono::system_clock::now(); //last dash time
        bool inDash = false; //check if in dash
        glm::vec3 dashDirection = glm::vec3(0, 0, 0); //dash direction
        float dashfov = 1.f; //dash fov
        float gravity = 23.0f; //gravity
        float upSpeed = 0.0f; //up speed
        float jumpSpeed = 10.0f; //jump speed
        // The ID of this component type is "Free Camera Controller"
        static std::string getID() { return "Free Camera Controller"; }

        // Reads sensitivities & speedupFactor from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}