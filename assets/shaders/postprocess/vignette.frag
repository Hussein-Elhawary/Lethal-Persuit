#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;

out vec4 frag_color;

// Vignette is a postprocessing effect that darkens the corners of the screen
// to grab the attention of the viewer towards the center of the screen

void main(){
    //TODO: Modify this shader to apply vignette
    // To apply vignette, divide the scene color
    // by 1 + the squared length of the 2D pixel location the NDC space
    // Hint: remember that the NDC space ranges from -1 to 1
    // while the texture coordinate space ranges from 0 to 1
    // We have the pixel's texture coordinate, how can we compute its location in the NDC space?
    if (tex_coord.x >= 0.492 && tex_coord.y >= 0.498 && tex_coord.x <= 0.508 && tex_coord.y <= 0.502) {
        frag_color = vec4(0.0,255.0,255.0,255.0);
        return;
    }
    if (tex_coord.x >= 0.499 && tex_coord.y >= 0.49 && tex_coord.x <= 0.501 && tex_coord.y <= 0.51) {
        frag_color = vec4(0.0,255.0,255.0,255.0);
        return;
    }
    frag_color = texture(tex, tex_coord);
    float dist = length(tex_coord * 2.0 - 1.0);
    frag_color /= 1.0 + dist * dist;
}