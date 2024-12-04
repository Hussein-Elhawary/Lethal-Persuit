#version 330 core

in Varyings {
    vec4 color;
    vec2 tex_coord;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;
uniform sampler2D Texture_0;
uniform sampler2D Texture_1;
uniform sampler2D Texture_2;
uniform sampler2D Texture_3;
uniform sampler2D Texture_4;
uniform sampler2D Texture_5;
uniform sampler2D Texture_6;


void main(){
    //TODO: (Req 7) Modify the following line to compute the fragment color
    // by multiplying the tint with the vertex color and with the texture color
    // fetch the pixel color from texture coordinate using the tex_coord and sampler
    // then multiply with the color and with the tint
    vec2 tex_coord = fs_in.tex_coord;
    vec4 street_map = texture(Texture_6, tex_coord);
    float pavement_factor = 1 - (street_map.r + street_map.b + street_map.g);

    tex_coord = fs_in.tex_coord * 25;
    vec4 pavement_diff = texture(Texture_2, tex_coord);
    vec4 pavement_disp = texture(Texture_3, tex_coord);
    vec4 pavement_color = pavement_diff*pavement_disp;

    tex_coord = fs_in.tex_coord * 50;
    vec4 tile = texture(Texture_0, tex_coord);
    vec4 tile_disp = texture(Texture_1, tex_coord);
    vec4 street_color = tile*tile_disp;

    vec4 separator_diff = texture(Texture_4, tex_coord);
    vec4 separator_disp = texture(Texture_5, tex_coord);
    vec4 separator_color = separator_diff*separator_disp;

    vec4 final_color = pavement_color*pavement_factor + street_color * street_map.b + street_map.r*separator_color;
    frag_color = final_color;
}  