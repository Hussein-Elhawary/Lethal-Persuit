#version 330 core

in Varyings {
    vec4 color;
    vec2 tex_coord;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;
uniform sampler2D tex;

void main(){
    //TODO: (Req 7) Modify the following line to compute the fragment color
    // by multiplying the tint with the vertex color and with the texture color
    // fetch the pixel color from texture coordinate using the tex_coord and sampler
    // then multiply with the color and with the tint
    vec4 tex_color = texture(tex, fs_in.tex_coord);
    if (tex_color.a == 0.0) discard;
    if (tex_color.r <= 0.3 && tex_color.g <= 0.2 && tex_color.b <= 0.15) discard;
    frag_color = tint * fs_in.color * texture(tex, fs_in.tex_coord);
}