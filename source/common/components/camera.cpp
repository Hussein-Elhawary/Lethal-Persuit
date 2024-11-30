#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace our {
    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json &data) {
        if (!data.is_object()) return;
        if (const std::string cameraTypeStr = data.value("cameraType", "perspective");
            cameraTypeStr == "orthographic") {
            cameraType = CameraType::ORTHOGRAPHIC;
        } else {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", 0.01f);
        far = data.value("far", 100.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
    }

    // Creates and returns the camera view matrix
    glm::mat4 CameraComponent::getViewMatrix() const {
        const auto owner = getOwner();
        const auto M = owner->getLocalToWorldMatrix();
        //TODO: (Req 8) Complete this function
        //HINT:
        // In the camera space:
        // - eye is the origin (0,0,0)
        // - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        // - up is the direction (0,1,0)
        // but to use glm::lookAt, we need eye, center and up in the world state.
        // Since M (see above) transforms from the camera to thw world space, you can use M to compute:
        // - the eye position which is the point (0,0,0) but after being transformed by M
        // - the center position which is the point (0,0,-1) but after being transformed by M
        // - the up direction which is the vector (0,1,0) but after being transformed by M
        // then you can use glm::lookAt


        // eye and center are coordinates thus homogenous component is equal to 1
        // up is a vector thus homogenous component is equal to 0
        // we have M which is transformation matrix from camera space to world space
        // we want camera coordinates in world space
        // so we multiply M with camera coordinates in camera space to get camera coordinates in world space
        const glm::vec4 eye4d = M * glm::vec4({0, 0, 0, 1});
        const glm::vec4 center4d = M * glm::vec4({0, 0, -1, 1});
        const glm::vec4 up4d = M * glm::vec4({0, 1, 0, 0});

        const auto eye = glm::vec3(eye4d.x, eye4d.y, eye4d.z);
        const auto center = glm::vec3(center4d.x, center4d.y, center4d.z);
        const auto up = glm::vec3(up4d.x, up4d.y, up4d.z);

        const glm::mat4 res = lookAt(eye, center, up);
        return res;
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(const glm::ivec2 viewportSize) const {
        //TODO: (Req 8) Write this function
        // NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        // It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        // Left and Right are the same but after being multiplied by the aspect ratio
        // For the perspective camera, you can use glm::perspective
        glm::mat4 res;
        if (cameraType == CameraType::ORTHOGRAPHIC) {
            const float aspectRatio = static_cast<float>(viewportSize.x) / static_cast<float>(viewportSize.y);
            const float halfHeight = orthoHeight / 2;
            const float halfWidth = aspectRatio * halfHeight;
            //args: left, right, bottom, top, near, far
            res = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, near, far);
        } else {
            res = glm::perspective(fovY, static_cast<float>(viewportSize.x) / static_cast<float>(viewportSize.y), near,
                                   far);
        }
        return res;
    }
}
