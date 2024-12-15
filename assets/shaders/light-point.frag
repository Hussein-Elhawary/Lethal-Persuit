#version 330

in Varyings {
    vec4 color;
    vec2 tex_coord;
    // We will need the vertex position in the world space, for specular component
    vec3 world;
    // the view vector (vertex to eye vector in the world space), for diffuse compoent
    vec3 view;
    // and the surface normal in the world space, for diffuse component
    vec3 normal;
} fsin;

// This contains all the material properties in a single struct.
struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
};

// This will include all the data we need about directional lights.
struct SpotLight {
    // These defines the colors and intensities of the light.
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;

    // Point lights only has a position. (No direction since it spreads in all directions).
    vec3 position;

    // The attenuation is used to control how the light dims out as we go further from it.
    float attenuation_constant;
    float attenuation_linear;
    float attenuation_quadratic;

};

// Receive the material and the light as uniforms.
uniform Material material;
uniform SpotLight light;

out vec4 frag_color;

void main() {
    // First we normalize the normal and the view.
    vec3 normal = normalize(fsin.normal); // Although the normal was already normalized, it may become shorter during interpolation.
    vec3 view = normalize(fsin.view);

    // Then we get the light direction and distance relative to the pixel location in the world space.
    vec3 light_direction = fsin.world - light.position;
    float distance = length(light_direction);
    light_direction /= distance; //normalize

    // calculate labert
    float lambert = max(0.0f, dot(normal, -light_direction));

    // Then we calculate the attenuation factor based on the light distance from the pixel.
    float attenuation = 1.0f / (light.attenuation_constant +
                                light.attenuation_linear * distance +
                                light.attenuation_quadratic * distance * distance);

    // calculate phong
    // for Specular = Ks * Is * Max(0, V · R)^⍺
    // so we want to calculate reflected light vector R
    vec3 reflected = reflect(light_direction, normal);
    float phong = pow(max(0.0f, dot(view, reflected)), material.shininess);

    // Now we compute the 3 components of the light separately.
    vec3 diffuse = material.diffuse * light.diffuse * lambert;
    vec3 specular = material.specular * light.specular * phong;
    vec3 ambient = material.ambient * light.ambient;

    // Then we combine the light component additively.
    // attenuation only affects the diffuse and specular since ambient should be constant regardless of the position and direction.
    frag_color = fsin.color * vec4((diffuse + specular) * attenuation + ambient, 1.0f);
    //frag_color = vec4(light.attenuation_constant,light.attenuation_linear,light.attenuation_quadratic, 1.0f);
}