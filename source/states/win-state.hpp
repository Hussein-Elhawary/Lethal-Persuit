#pragma once

#ifndef WIN_STATE_HPP
#define WIN_STATE_HPP
#include <chrono>

#include "application.hpp"
#include "material/material.hpp"
#include "mesh/mesh.hpp"
#include "texture/texture-utils.hpp"

class WinState : public our::State {
    our::TexturedMaterial *menuMaterial = nullptr;
    our::TintedMaterial *highlightMaterial = nullptr;
    our::Mesh *rectangle = nullptr;
    float time = 0;
    std::chrono::system_clock::time_point timeEntered;

    void onInitialize() override {
        menuMaterial = new our::TexturedMaterial();
        menuMaterial->shader = new our::ShaderProgram();
        menuMaterial->shader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
        menuMaterial->shader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
        menuMaterial->shader->link();
        menuMaterial->texture = our::texture_utils::loadImage("assets/textures/win.jpg");

        menuMaterial->tint = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

        highlightMaterial = new our::TintedMaterial();
        highlightMaterial->shader = new our::ShaderProgram();
        highlightMaterial->shader->attach("assets/shaders/tinted.vert", GL_VERTEX_SHADER);
        highlightMaterial->shader->attach("assets/shaders/tinted.frag", GL_FRAGMENT_SHADER);
        highlightMaterial->shader->link();
        highlightMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        highlightMaterial->pipelineState.blending.enabled = true;
        highlightMaterial->pipelineState.blending.equation = GL_FUNC_SUBTRACT;
        highlightMaterial->pipelineState.blending.sourceFactor = GL_ONE;
        highlightMaterial->pipelineState.blending.destinationFactor = GL_ONE;

        rectangle = new our::Mesh({
                                      {{0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
                                      {{1.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
                                      {{1.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                                      {{0.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                                  }, {
                                      0, 1, 2, 2, 3, 0,
                                  });

        time = 0;
        timeEntered = std::chrono::system_clock::now();
    }

    void onDraw(const double deltaTime) override {
        if (const auto timeSinceEntered = std::chrono::duration<float>(std::chrono::system_clock::now() - timeEntered).count(); timeSinceEntered > 10.0f) {
            getApp()->changeState("menu");
        }

        if (const auto &keyboard = getApp()->getKeyboard(); keyboard.justPressed(GLFW_KEY_SPACE)) {
            getApp()->changeState("play");
        } else if (keyboard.justPressed(GLFW_KEY_ESCAPE)) {
            getApp()->changeState("menu");
        }

        // Get the framebuffer size to set the viewport and the create the projection matrix.
        const glm::ivec2 size = getApp()->getFrameBufferSize();
        // Make sure the viewport covers the whole size of the framebuffer.
        glViewport(0, 0, size.x, size.y);

        glm::mat4 VP = glm::ortho(0.0f, static_cast<float>(size.x), static_cast<float>(size.y), 0.0f, 1.0f, -1.0f);
        const glm::mat4 M = glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

        // First, we apply the fading effect.
        time += static_cast<float>(deltaTime);
        menuMaterial->tint = glm::vec4(glm::smoothstep(0.00f, 2.00f, time));

        menuMaterial->setup();
        menuMaterial->shader->set("transform", VP * M);
        rectangle->draw();
    }

    void onDestroy() override {
        // Delete all the allocated resources
        delete rectangle;
        delete menuMaterial->texture;
        delete menuMaterial->shader;
        delete menuMaterial;
        delete highlightMaterial->shader;
        delete highlightMaterial;
    }
};

#endif //WIN_STATE_HPP
