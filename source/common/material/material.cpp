#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

// The goal of requirement 7 is to specify the material that object can have
// Where a material means: 
// 1. what options to be enabled for the object and this is achieved from pipelineState
// 2. what ate the shaders of the object so we activate the program that contains the correct shaders
// 3. if the material is just a single color you can just supply the fragment shader
// with a factor to multiply the color with the number
// 4. if the material is a texture so we bind the texture and sampler to unit 0
// and set a uniform with the unit 0 to pass the unit number to the fragment shader
// to be used in sampling process

namespace our {
    // This function should set up the pipeline state and set the shader to be used
    void Material::setup() const {
        //TODO: (Req 7) Write this function

        // we set up the pipeline state which will set the required options for a material
        // like backface culling, depth testing, blending and so on
        pipelineState.setup();
        // use the shader program where a shader program contains the fragment and
        // vertex shader that will be used to render the object
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json &data) {
        if (!data.is_object()) return;

        if (data.contains("pipelineState")) {
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //TODO: (Req 7) Write this function
        Material::setup();
        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json &data) {
        Material::deserialize(data);
        if (!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        //TODO: (Req 7) Write this function
        TintedMaterial::setup();
        shader->set("alphaThreshold", alphaThreshold);
        // we have only one texture per object so we set the active unit to 0 and keep it active for the rest of the frame
        // we have multiple texture units, so we need to specify which unit we are using
        // we use unit 0 here
        glActiveTexture(GL_TEXTURE0);
        // bind the 2D texture to the active texture unit
        texture->bind();
        // bind the sampler to the active texture unit
        sampler->bind(0);
        // set the uniform variable "tex" to the active texture unit
        shader->set("tex", 0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json &data) {
        TintedMaterial::deserialize(data);
        if (!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }
}
