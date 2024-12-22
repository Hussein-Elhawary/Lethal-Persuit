#version 330

// The texture holding the scene pixels
uniform sampler2D tex;
uniform sampler2D depthTex;

in vec2 tex_coord;
out vec4 frag_color;

#define STRENGTH 0.001

void main(){
    float current_depth = texture(depthTex, tex_coord).r;

    float blur = (current_depth - 0.9) / (1 - 0.9) * STRENGTH;

    vec4 color = texture(tex, tex_coord);
    color += texture(tex, tex_coord + vec2(blur, 0.0));
    color += texture(tex, tex_coord + vec2(-blur, 0.0));
    color += texture(tex, tex_coord + vec2(0.0, blur));
    color += texture(tex, tex_coord + vec2(0.0, -blur));
    color += texture(tex, tex_coord + vec2(blur, blur));
    color += texture(tex, tex_coord + vec2(-blur, -blur));
    color += texture(tex, tex_coord + vec2(-blur, blur));
    color += texture(tex, tex_coord + vec2(blur, -blur));
    color /= 9.0;

    frag_color = color;
}