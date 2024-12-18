#include "mesh-utils.hpp"

// We will use "Tiny OBJ Loader" to read and process '.obj" files
#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobj/tiny_obj_loader.h>

#include <iostream>
#include <vector>
#include <unordered_map>

#include "objects/Object.hpp"

std::vector<our::Mesh *> *our::mesh_utils::loadOBJ(const std::string &filename) {
    // The data loaded by Tiny OBJ Loader
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str())) {
        std::cerr << "Failed to load obj file \"" << filename << "\" due to error: " << err << std::endl;
        return {};
    }
    if (!warn.empty()) {
        std::cout << "WARN while loading obj file \"" << filename << "\": " << warn << std::endl;
    }

    // An obj file can have multiple shapes where each shape can have its own material
    // Ideally, we would load each shape into a separate mesh or store the start and end of it in the element buffer to be able to draw each shape separately
    // But we ignored this fact since we don't plan to use multiple materials in the examples
    // Initialize the meshes vector with the number of shapes
    auto *meshes = new std::vector<Mesh *>();
    for (const auto &[name, mesh, lines, points]: shapes) {
        // The data that we will use to initialize our mesh
        std::vector<Vertex> vertices;
        std::vector<GLuint> elements;
        // std::string material_name = materials[mesh.material_ids[0]].name + "_material";


        // Since the OBJ can have duplicated vertices, we make them unique using this map
        // The key is the vertex, the value is its index in the vector "vertices".
        // That index will be used to populate the "elements" vector.
        std::unordered_map<Vertex, GLuint> vertex_map;

        for (const auto &[vertex_index, normal_index, texcoord_index]: mesh.indices) {
            Vertex vertex = {};

            // Read the data for a vertex from the "attrib" object
            vertex.position = {
                attrib.vertices[3 * vertex_index + 0],
                attrib.vertices[3 * vertex_index + 1],
                attrib.vertices[3 * vertex_index + 2]
            };

            vertex.normal = {
                attrib.normals[3 * normal_index + 0],
                attrib.normals[3 * normal_index + 1],
                attrib.normals[3 * normal_index + 2]
            };

            vertex.tex_coord = {
                attrib.texcoords[2 * texcoord_index + 0],
                attrib.texcoords[2 * texcoord_index + 1]
            };

            vertex.color = {
                attrib.colors[3 * vertex_index + 0] * 255,
                attrib.colors[3 * vertex_index + 1] * 255,
                attrib.colors[3 * vertex_index + 2] * 255,
                255
            };

            // See if we already stored a similar vertex
            auto it = vertex_map.find(vertex);
            if (it == vertex_map.end()) {
                // if no, add it to the vertices and record its index
                auto new_vertex_index = static_cast<GLuint>(vertices.size());
                vertex_map[vertex] = new_vertex_index;
                elements.push_back(new_vertex_index);
                vertices.push_back(vertex);
            } else {
                // if yes, just add its index in the elements vector
                elements.push_back(it->second);
            }
        }

        meshes->push_back(new Mesh(vertices, elements));
    }

    return meshes;
}


std::vector<our::Mesh *> *our::mesh_utils::loadOBJ(const Object &object) {
    auto &attrib = object.attrib;
    auto &shapes = object.shapes;
    auto &materials = object.materials;
    auto *meshes = new std::vector<Mesh *>();
    for (const auto &[name, mesh, lines, points]: shapes) {
        // The data that we will use to initialize our mesh
        std::vector<Vertex> vertices;
        std::vector<GLuint> elements;
        std::string materialName;
        if (!materials.empty()) {
            materialName = materials[mesh.material_ids[0]].name + "_material";
        }


        // Since the OBJ can have duplicated vertices, we make them unique using this map
        // The key is the vertex, the value is its index in the vector "vertices".
        // That index will be used to populate the "elements" vector.
        std::unordered_map<Vertex, GLuint> vertex_map;

        for (const auto &[vertex_index, normal_index, texcoord_index]: mesh.indices) {
            Vertex vertex = {};

            // Read the data for a vertex from the "attrib" object
            vertex.position = {
                attrib.vertices[3 * vertex_index + 0],
                attrib.vertices[3 * vertex_index + 1],
                attrib.vertices[3 * vertex_index + 2]
            };

            vertex.normal = {
                attrib.normals[3 * normal_index + 0],
                attrib.normals[3 * normal_index + 1],
                attrib.normals[3 * normal_index + 2]
            };

            vertex.tex_coord = {
                attrib.texcoords[2 * texcoord_index + 0],
                attrib.texcoords[2 * texcoord_index + 1]
            };


            vertex.color = {
                attrib.colors[3 * vertex_index + 0] * 255,
                attrib.colors[3 * vertex_index + 1] * 255,
                attrib.colors[3 * vertex_index + 2] * 255,
                255
            };

            // See if we already stored a similar vertex
            auto it = vertex_map.find(vertex);
            if (it == vertex_map.end()) {
                // if no, add it to the vertices and record its index
                auto new_vertex_index = static_cast<GLuint>(vertices.size());
                vertex_map[vertex] = new_vertex_index;
                elements.push_back(new_vertex_index);
                vertices.push_back(vertex);
            } else {
                // if yes, just add its index in the elements vector
                elements.push_back(it->second);
            }
        }

        meshes->push_back(new Mesh(vertices, elements, materialName));
    }

    return meshes;
}


// Create a sphere (the vertex order in the triangles are CCW from the outside)
// Segments define the number of divisions on the both the latitude and the longitude
our::Mesh *our::mesh_utils::sphere(const glm::ivec2 &segments) {
    std::vector<our::Vertex> vertices;
    std::vector<GLuint> elements;

    // We populate the sphere vertices by looping over its longitude and latitude
    for (int lat = 0; lat <= segments.y; lat++) {
        float v = (float) lat / segments.y;
        float pitch = v * glm::pi<float>() - glm::half_pi<float>();
        float cos = glm::cos(pitch), sin = glm::sin(pitch);
        for (int lng = 0; lng <= segments.x; lng++) {
            float u = (float) lng / segments.x;
            float yaw = u * glm::two_pi<float>();
            glm::vec3 normal = {cos * glm::cos(yaw), sin, cos * glm::sin(yaw)};
            glm::vec3 position = normal;
            glm::vec2 tex_coords = glm::vec2(u, v);
            our::Color color = our::Color(255, 255, 255, 255);
            vertices.push_back({position, color, tex_coords, normal});
        }
    }

    for (int lat = 1; lat <= segments.y; lat++) {
        int start = lat * (segments.x + 1);
        for (int lng = 1; lng <= segments.x; lng++) {
            int prev_lng = lng - 1;
            elements.push_back(lng + start);
            elements.push_back(lng + start - segments.x - 1);
            elements.push_back(prev_lng + start - segments.x - 1);
            elements.push_back(prev_lng + start - segments.x - 1);
            elements.push_back(prev_lng + start);
            elements.push_back(lng + start);
        }
    }

    return new our::Mesh(vertices, elements);
}
