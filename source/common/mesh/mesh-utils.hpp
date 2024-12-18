#pragma once

#include "mesh.hpp"
#include <string>
#include <tinyobj/tiny_obj_loader.h>

#include "objects/Object.hpp"

namespace our::mesh_utils {
    // Load an ".obj" file into the mesh
    std::vector<Mesh *> *loadOBJ(const std::string &filename);
    std::vector<Mesh *> *loadOBJ(const Object &object
    );

    // Create a sphere (the vertex order in the triangles are CCW from the outside)
    // Segments define the number of divisions on the both the latitude and the longitude
    Mesh *sphere(const glm::ivec2 &segments);
}
