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


void main(){
    
    vec2 tex_coord = fs_in.tex_coord;
    vec4 final_color = vec4(0.0, 0.0, 0.0, 1.0);
    // if(tex_coord.x > 0.6 && tex_coord.x < 0.92 && tex_coord.y > 0.65 && tex_coord.y < 0.85)
    // {
    //     tex_coord = vec2((tex_coord.x-0.6) * 3.125, (tex_coord.y-0.65) * 5);
    //     final_color = texture(Texture_3, tex_coord);
    //     if (final_color.r+ final_color.g + final_color.b < 0.08 )
    //     {
    //         vec4 wall_disp = texture(Texture_0, tex_coord);
    //         vec4 wall_diff = texture(Texture_1, tex_coord);
    //         vec4 wall_color = texture(Texture_2, tex_coord);
    //         final_color = wall_color*wall_disp*wall_diff;
    //     }
    // }
    if (tex_coord.x > 0.15 && tex_coord.x < 0.35 && tex_coord.y > 0 && tex_coord.y < 0.5)
    {
        tex_coord = vec2((tex_coord.x-0.15) * 5, (tex_coord.y) * 2);
        final_color = texture(Texture_4, tex_coord);

    }
    else
    {
        vec4 wall_disp = texture(Texture_0, tex_coord);
        vec4 wall_diff = texture(Texture_1, tex_coord);
        vec4 wall_color = texture(Texture_2, tex_coord);
        final_color = wall_color*wall_disp*wall_diff;
    }
    frag_color = final_color;
}  