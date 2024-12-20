#include "weapon.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    void Weapon::deserialize(const nlohmann::json &data) {
        initialPosition = data.value("initialPosition", initialPosition);
    }
} // our