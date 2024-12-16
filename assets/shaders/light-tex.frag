#version 330

/**
We have three type of lights: Directional, Point and Spot.
For directional we want only the direction
since it is like the sun the rays are parallel.
For point we want the position of the light
since it spreads light in all directions.
For spot we want the position and the direction
since it spreads light in a cone shape.
*/

in Varyings {
    vec4 color;
    vec2 tex_coord;
    // We will need the vertex position in the world space, for specular component
    vec3 world;
    // the view vector (vertex to eye vector in the world space), for diffuse compoent
    vec3 view;
    // and the surface normal in the world space, for diffuse component in lambert calculation
    vec3 normal;
} fsin;

// This contains all the material properties in a single struct.
struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    vec3 emissive;
    float shininess;
};

struct TexturedMaterial {
    sampler2D albedo_map;
    sampler2D specular_map;
    sampler2D ambient_occlusion_map;
    sampler2D roughness_map;
    sampler2D emissive_map;
};

#define DIRECTIONAL_LIGHT_TYPE    0
#define POINT_LIGHT_TYPE          1
#define SPOT_LIGHT_TYPE           2

// This will include all the data we need about lights.
struct Light {
    // The light type.
    int type;

    // These defines the colors of the light.
    vec3 color;

    // For directional light and spot light, we need the direction.
    vec3 direction;

    // For point light and spot light, we need the position.
    vec3 position;

    // The attenuation is used to control how the light dims out as we go further from it.
    float attenuation_constant;
    float attenuation_linear;
    float attenuation_quadratic;

    // For spot lights, we also need the angles to define the cone shape.
    float inner_angle, outer_angle;
};

// Receive the material and the light as uniforms.
uniform TexturedMaterial material;

uniform vec3 ambient_light;

#define MAX_LIGHT_COUNT 16

uniform Light lights[MAX_LIGHT_COUNT];
uniform int light_count;
out vec4 frag_color;

// This function takes a texturedMaterial and returns a material
// after sampling the textures.

// Material sample_material(TexturedMaterial texturedMaterial, vec2 tex_coord){
//     Material material;

//     //sample textures from maps
//     material.diffuse = texture(texturedMaterial.albedo_map, tex_coord).rgb;
//     material.specular = texture(texturedMaterial.specular_map, tex_coord).rgb;
//     material.ambient = material.diffuse*texture(texturedMaterial.ambient_occlusion_map, tex_coord).rgb;
//     material.emissive = texture(texturedMaterial.emissive_map, tex_coord).rgb;
//     float roughness = texture(texturedMaterial.roughness_map, tex_coord).r;

//     //convert roughness to shininess
//     //shininess = 2/roughness^4 - 2 => add clamp to avoid division by zero
//     material.shininess = 2.0f/pow(clamp(roughness, 0.001f, 0.999f), 4.0f) - 2.0f;
//     return material;
// }

void main() {
    // // First we normalize the normal and the view.
    // vec3 normal = normalize(fsin.normal); // Although the normal was already normalized, it may become shorter during interpolation.
    // vec3 view = normalize(fsin.view);


    // int count = min(light_count, MAX_LIGHT_COUNT);

    // Material sampledMaterial = sample_material(material, fsin.tex_coord);

    // //ambient light and emissive light are constant and not affected by light position
    // //and not affected by attenuation
    // vec3 light_mixed_color = sampledMaterial.emissive + sampledMaterial.ambient * ambient_light;
    
    // for(int i = 0; i < count; i++) {
    //     Light light = lights[i];
    //     vec3 light_to_object_direction;
    //     float distance;
    //     if(light.type == DIRECTIONAL_LIGHT_TYPE){
    //         light_to_object_direction = light.direction;
    //     }else if(light.type == POINT_LIGHT_TYPE || light.type == SPOT_LIGHT_TYPE){
    //         light_to_object_direction = fsin.world - light.position;
    //         distance = length(light_to_object_direction);
    //         light_to_object_direction /= distance; //normalize
    //     }

    //     float attenuation = 1.0f;
    //     if(light.type == POINT_LIGHT_TYPE || light.type == SPOT_LIGHT_TYPE){
    //         // Then we calculate the attenuation factor based on the light distance from the pixel.
    //         float attenuation = 1.0f / (light.attenuation_constant +
    //         light.attenuation_linear * distance +
    //         light.attenuation_quadratic * distance * distance);
    //     }

    //     float angle_attenuation = 1.0f;
    //     if(light.type == SPOT_LIGHT_TYPE){
    //         // Then we calculate the angle between the pixel and the cone axis.
    //         float angle = acos(dot(light.direction, light_to_object_direction));
    //         // And we calculate the attenuation based on the angle.
    //         angle_attenuation = smoothstep(light.outer_angle, light.inner_angle, angle);
    //     }

    //     float lambert = max(0.0f, dot(normal, -light_to_object_direction));

    //     // calculate phong
    //     // for Specular = Ks * Is * Max(0, V · R)^⍺
    //     // so we want to calculate reflected light vector R
    //     vec3 reflected = reflect(light_to_object_direction, normal);
    //     float phong = pow(max(0.0f, dot(view, reflected)), sampledMaterial.shininess);

    //     // Now we compute the 3 components of the light separately.
    //     vec3 diffuse = sampledMaterial.diffuse * light.color * lambert;
    //     vec3 specular = sampledMaterial.specular * light.color * phong;

    //     // Then we combine the light component additively.
    //     // attenuation only affects the diffuse and specular since ambient should be constant regardless of the position and direction.
    //     light_mixed_color += (diffuse + specular) * attenuation * angle_attenuation;
    // }

    // //frag_color = fsin.color * vec4(light_mixed_color, 1.0f);
    // // frag_color = vec4(texture(texturedMaterial.emissive_map, tex_coord).rgb, 1.0f);
    frag_color = vec4(1.0f,1.0f,1.0f, 1.0f);
}