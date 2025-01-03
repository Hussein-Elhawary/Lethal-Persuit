#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the top-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){

    // Calculate the x and y coordinates of the current fragment in terms of the checkerboard tiles
    int x = int(gl_FragCoord.x) / size;
    int y = int(gl_FragCoord.y) / size;

    // Determine the color of the current tile based on the sum of x and y
    // If the sum is odd, use the second color if not, use the first color
    int id = (x + y) % 2;
    frag_color = vec4(colors[id], 1.0);


}