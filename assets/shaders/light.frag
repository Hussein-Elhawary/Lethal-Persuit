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
struct DirectionalLight {
    // These defines the colors and intensities of the light.
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;

    // Directional light are only defined by a direction. (It has no position).
    vec3 direction;
};

// Receive the material and the light as uniforms.
uniform Material material;
uniform DirectionalLight light;

uniform vec3 camera_position;
uniform mat4 object_to_world_inv_transpose;

out vec4 frag_color;

void main() {
    // First we normalize the normal and the view.
    vec3 normal = normalize(fsin.normal); // Although the normal was already normalized, it may become shorter during interpolation.
    vec3 view = normalize(fsin.view);

    // calculate labert
    float lambert = max(0.0f, dot(normal, -light.direction));

    // calculate phong
    // for Specular = Ks * Is * Max(0, V · R)^⍺
    // so we want to calculate reflected light vector R
    vec3 reflected = reflect(light.direction, normal);
    float phong = pow(max(0.0f, dot(view, reflected)), material.shininess);

    // Now we compute the 3 components of the light separately.
    vec3 diffuse = material.diffuse * light.diffuse * lambert;
    vec3 specular = material.specular * light.specular * phong;
    vec3 ambient = material.ambient * light.ambient;

    // Then we combine the light component additively.
    frag_color = fsin.color * vec4(diffuse + specular + ambient, 1.0f);
    //extract first row of object_to_world_inv_transpose in vec3
    //frag_color = vec4(object_to_world_inv_transpose[1][0],object_to_world_inv_transpose[1][1],object_to_world_inv_transpose[1][2], 1.0f);
}