
#include "Object.hpp"

#include <iostream>

namespace our {
    Object::Object(const std::string &filename) {
        std::string warn, err;

        if (!LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) {
            std::cerr << "Failed to load obj file \"" << filename << "\" due to error: " << err << std::endl;
            exit(EXIT_FAILURE);
        }
        if (!warn.empty()) {
            std::cout << "WARN while loading obj file \"" << filename << "\": " << warn << std::endl;
        }
    }
} // our
