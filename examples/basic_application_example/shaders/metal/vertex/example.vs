#version 330

layout(location = 0) in vec2 i_vert;
layout(location = 1) in vec3 i_color;
out vec3 v_color;    // Goes to the fragment shader

void main() {
    gl_Position = vec4(i_vert, 0.0, 1.0);
    v_color = i_color;
}
