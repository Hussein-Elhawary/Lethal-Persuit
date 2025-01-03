#pragma once

#include "pipeline-state.hpp"
#include "../texture/texture2d.hpp"
#include "../texture/sampler.hpp"
#include "../shader/shader.hpp"

#include <glm/vec4.hpp>
#include <json/json.hpp>
#include <vector>
#include <tinyobj/tiny_obj_loader.h>

namespace our {

    // This is the base class for all the materials
    // It contains the 3 essential components required by any material
    // 1- The pipeline state when drawing objects using this material
    // 2- The shader program used to draw objects using this material
    // 3- Whether this material is transparent or not
    // Materials that send uniforms to the shader should inherit from the is material and add the required uniforms
    class Material {
    public:
        PipelineState pipelineState;
        ShaderProgram* shader;
        bool transparent;
        
        // This function does 2 things: setup the pipeline state and set the shader program to be used
        virtual void setup() const;
        // This function read a material from a json object
        virtual void deserialize(const nlohmann::json& data);
    };

    // This material adds a uniform for a tint (a color that will be sent to the shader)
    // An example where this material can be used is when the whole object has only color which defined by tint
    class TintedMaterial : public Material {
    public:
        glm::vec4 tint;

        void setup() const override;
        void deserialize(const nlohmann::json& data) override;
    };

    // This material adds two uniforms (besides the tint from Tinted Material)
    // The uniforms are:
    // - "tex" which is a Sampler2D. "texture" and "sampler" will be bound to it.
    // - "alphaThreshold" which defined the alpha limit below which the pixel should be discarded
    // An example where this material can be used is when the object has a texture
    class TexturedMaterial : public TintedMaterial {
    public:
        Texture2D* texture;
        Sampler* sampler;
        float alphaThreshold;

        void setup() const override;
        void deserialize(const nlohmann::json& data) override;
    };

    class LitMaterial: public Material {
    public:
        //Diffuse: release of light absorbed by a material in all directions with same amount
        //in all directions.
        //It depends on angle of incidence on the pixel
        //Specular: reflection of light by material in multiple directions but intensity
        //according to the angle with eye
        //Ambient: light emitted by default even if no source
        //Shininess: Related to specular, it is related to roughness of the surface
        //more rough material thus less concentrated rays reflected
        glm::vec3 diffuse, specular, ambient;
        float shininess;

        void setup() const override;
        void deserialize(const nlohmann::json& data) override;
    };

    class LitTexturedMaterial : public Material {
    public:
        Texture2D *albedoTexture; //diffuse
        Sampler* albedoSampler;

        Texture2D *specularTexture; //mettalic
        Sampler* specularSampler;

        Texture2D *ambientOcclusionTexture; //ao
        Sampler * ambientOcclusionSampler;

        Texture2D *roughnessTexture; //roughness
        Sampler *roughnessSampler;

        Texture2D* emissiveTexture; //emissive
        Sampler *emissiveSampler;

        void setup() const override;
        void deserialize(const nlohmann::json& data) override;
        void loadTextures(const tinyobj::material_t &data);
        void loadTexture(const std::string &textureName, Texture2D *&texture, Sampler *&sampler);
        void load(const tinyobj::material_t &data);

    private:
        void setupTexture(Texture2D* texture, Sampler* sampler, const std::string& uniformName, uint64_t activeTexture) const;
        void deserializeTexture(const nlohmann::json& data, Texture2D*& texture, Sampler*& sampler);
    };

    class NTexturedMaterial : public TintedMaterial {
    public:
        std::vector<Texture2D*> textures;
        std::vector<Sampler*> samplers;
        float alphaThreshold;
        
        void setup() const override;
        void deserialize(const nlohmann::json& data) override;
    };

    // This function returns a new material instance based on the given type
    inline Material* createMaterialFromType(const std::string& type){
        if(type == "tinted"){
            return new TintedMaterial();
        } else if(type == "textured"){
            return new TexturedMaterial();
        } else if(type == "Ntextured"){
            return new NTexturedMaterial();
        }else if(type == "LitMaterial") {
            return new LitMaterial();
        }else if(type == "LitTexturedMaterial") {
            return new LitTexturedMaterial();
        }
        else{
            return new Material();
        }
    }

}
