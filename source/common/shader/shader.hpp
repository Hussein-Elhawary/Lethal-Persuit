#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {
    class ShaderProgram {
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram() {
            //TODO: (Req 1) Create A shader program
            program = glCreateProgram();
        }
        ~ShaderProgram() {
            //TODO: (Req 1) Delete a shader program
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() const {
            glUseProgram(program);
        }

        GLint getUniformLocation(const std::string &name) const {
            //TODO: (Req 1) Return the location of the uniform with the given name
            return glGetUniformLocation(program, name.c_str());
        }

        void set(const std::string &uniform, const GLfloat value) const {
            //TODO: (Req 1) Send the given float value to the given uniform
            glUniform1f(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, const GLuint value) const {
            //TODO: (Req 1) Send the given unsigned integer value to the given uniform
            glUniform1ui(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, const GLint value) const {
            //TODO: (Req 1) Send the given integer value to the given uniform
            glUniform1i(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, glm::vec2 value) const {
            //TODO: (Req 1) Send the given 2D vector value to the given uniform
            glUniform2fv(getUniformLocation(uniform), 1, value_ptr(value));
        }

        void set(const std::string &uniform, glm::vec3 value) const {
            //TODO: (Req 1) Send the given 3D vector value to the given uniform
            glUniform3fv(getUniformLocation(uniform), 1, value_ptr(value));
        }

        void set(const std::string &uniform, glm::vec4 value) const {
            //TODO: (Req 1) Send the given 4D vector value to the given uniform
            glUniform4fv(getUniformLocation(uniform), 1, value_ptr(value));
        }

        void set(const std::string &uniform, glm::mat4 matrix) const {
            //TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            glUniformMatrix4fv(getUniformLocation(uniform), 1, GL_FALSE, value_ptr(matrix));
        }

        //TODO: (Req 1) Delete the copy constructor and assignment operator.
        ShaderProgram(const ShaderProgram &) = delete;
        ShaderProgram &operator=(const ShaderProgram &) = delete;
        // Deleting the copy constructor and assignment operator prevents copying of the Shader class.
        // This is important because the Shader class manages resources (e.g., GPU shaders) 
        // that should not be duplicated. Copying could lead to resource leaks or other issues.

        //Question: Why do we delete the copy constructor and assignment operator?
    };
}

#endif
