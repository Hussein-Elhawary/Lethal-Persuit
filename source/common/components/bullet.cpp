#include "bullet.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    void Bullet::deserialize(const nlohmann::json &data) {
        direction = data.value("direction", direction);
        speed = data.value("speed", speed);
    }
} // our