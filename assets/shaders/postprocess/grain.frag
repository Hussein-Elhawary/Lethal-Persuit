#version 330
uniform sampler2D tex;
uniform float grain_amount = 0.05; // Adjust the amount of grain
uniform float grain_size = 1.0; // Adjust the size of the grain

in vec2 tex_coord;
out vec4 frag_color;

void main() {
    // Sample the original screen texture
    vec4 original_color = texture(tex, tex_coord);

    float noise = 0.0;
    noise = (fract(sin(dot(tex_coord, vec2(12.9898, 78.233))) * 43758.5453) - 0.5) * 2.0;
    // Add noise to the original color
    original_color.rgb += noise * grain_amount * grain_size;

    // Clamp the final color to make sure it stays in the valid range
    frag_color = clamp(original_color, 0.0, 1.0);
}
