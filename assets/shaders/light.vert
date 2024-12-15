#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
// going to add the normal because it will be used in calculations in
// the fragement shader
layout(location = 3) in vec3 normal;

// break transformation into
// 1. object to world space (beacause all light calculations are done in world space)
//      -> will include also object to world space inverse transpose matrix
//      -> to perform calculations on the normal vector correctly
uniform mat4 object_to_world;
uniform mat4 object_to_world_inv_transpose;

// 2. world to homogenous clip space using view matrix (to calculate the coordinates of the object)
uniform mat4 view_projection;

// Camera position which will be used in specular component calculation
uniform vec3 camera_position;

out Varyings {
    vec4 color;
    vec2 tex_coord;
    // Send the vertex position in the world space
    vec3 world;
    // View vector (vertex to eye vector in the world space)
    vec3 view;
    // and the surface normal in the world space.
    vec3 normal;
} vsout;

void main() {
    // Compute the world position of the object
    vsout.world = (object_to_world * vec4(position, 1.0f)).xyz;

    // Compute the view vector (vertex to eye vector in the world space) to be used for specular computation later.
    vsout.view = camera_position - vsout.world;

    // Compute normal in the world space (Note that w=0 since this is a vector).
    vsout.normal = normalize((object_to_world_inv_transpose * vec4(normal, 0.0f)).xyz);

    // Compute the position in the homogenous clip space and send the rest of the data.
    gl_Position = view_projection * vec4(vsout.world, 1.0);

    vsout.color = color;
    vsout.tex_coord = tex_coord;
}