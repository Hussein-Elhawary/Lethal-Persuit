#pragma once

#include <string>
#include <glad/gl.h>
#include "vertex.hpp"

namespace our {
#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_COLOR    1
#define ATTRIB_LOC_TEXCOORD 2
#define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO{}, EBO{};
        unsigned int VAO{};
        // We need to remember the number of elements that will be drawn by glDrawElements
        GLsizei elementCount;

    public:
        std::string material;
        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &elements,
             const std::string &materialName = "") {
            //TODO: (Req 2) Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc

            material = materialName;

            elementCount = static_cast<GLsizei>(elements.size());

            // Generate and bind the VAO
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            // Generate and bind the VBO, then upload the vertex data
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size() * sizeof(Vertex)), vertices.data(),
                         GL_STATIC_DRAW);

            // Generate and bind the EBO, then upload the element data
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(elements.size() * sizeof(unsigned int)),
                         elements.data(), GL_STATIC_DRAW);

            // Set up the vertex attribute pointers
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  static_cast<void *>(nullptr));

            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
                                  reinterpret_cast<void *>(offsetof(Vertex, color)));

            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  reinterpret_cast<void *>(offsetof(Vertex, tex_coord)));

            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                                  reinterpret_cast<void *>(offsetof(Vertex, normal)));

            // Unbind the VAO
            glBindVertexArray(0);
        }

        // this function should render the mesh
        void draw() const {
            //TODO: (Req 2) Write this function
            // Bind the VAO
            glBindVertexArray(VAO);
            // Draw the elements
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, static_cast<void *>(nullptr));
            // Unbind the VAO
            glBindVertexArray(0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh() {
            //TODO: (Req 2) Write this function
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteVertexArrays(1, &VAO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };
}
