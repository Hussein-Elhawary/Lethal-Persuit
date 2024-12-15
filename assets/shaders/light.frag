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
    float shininess;
};

#define DIRECTIONAL_LIGHT_TYPE    0
#define POINT_LIGHT_TYPE          1
#define SPOT_LIGHT_TYPE           2

// This will include all the data we need about lights.
struct Light {
    // The light type.
    int type;

    // These defines the colors and intensities of the light.
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;

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
uniform Material material;

#define MAX_LIGHT_COUNT 16

uniform Light lights[MAX_LIGHT_COUNT];
uniform int light_count;
out vec4 frag_color;

void main() {
    // First we normalize the normal and the view.
    vec3 normal = normalize(fsin.normal); // Although the normal was already normalized, it may become shorter during interpolation.
    vec3 view = normalize(fsin.view);

    vec3 light_mixed_color = vec3(0.0f,0.0f,0.0f);

    int count = min(light_count, MAX_LIGHT_COUNT);

    for(int i = 0; i < count; i++) {
        Light light = lights[i];
        vec3 light_to_object_direction;
        float distance;
        if(light.type == DIRECTIONAL_LIGHT_TYPE){
            light_to_object_direction = light.direction;
        }else if(light.type != DIRECTIONAL_LIGHT_TYPE){
            light_to_object_direction = fsin.world - light.position;
            distance = length(light_to_object_direction);
            light_to_object_direction /= distance; //normalize
        }

        float attenuation = 1.0f;
        if(light.type == POINT_LIGHT_TYPE || light.type == SPOT_LIGHT_TYPE){
            // Then we calculate the attenuation factor based on the light distance from the pixel.
            float attenuation = 1.0f / (light.attenuation_constant +
            light.attenuation_linear * distance +
            light.attenuation_quadratic * distance * distance);
        }

        float angle_attenuation = 1.0f;
        if(light.type == SPOT_LIGHT_TYPE){
            // Then we calculate the angle between the pixel and the cone axis.
            float angle = acos(dot(light.direction, light_to_object_direction));
            // And we calculate the attenuation based on the angle.
            angle_attenuation = smoothstep(light.outer_angle, light.inner_angle, angle);
        }

        float lambert = max(0.0f, dot(normal, -light_to_object_direction));

        // calculate phong
        // for Specular = Ks * Is * Max(0, V · R)^⍺
        // so we want to calculate reflected light vector R
        vec3 reflected = reflect(light_to_object_direction, normal);
        float phong = pow(max(0.0f, dot(view, reflected)), material.shininess);

        // Now we compute the 3 components of the light separately.
        vec3 diffuse = material.diffuse * light.diffuse * lambert;
        vec3 specular = material.specular * light.specular * phong;
        vec3 ambient = material.ambient * light.ambient;

        // Then we combine the light component additively.
        // attenuation only affects the diffuse and specular since ambient should be constant regardless of the position and direction.
        light_mixed_color += (diffuse + specular) * attenuation * angle_attenuation + ambient;
    }

    frag_color = fsin.color * vec4(light_mixed_color, 1.0f);
}