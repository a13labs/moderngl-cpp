#version 330

layout(location = 0) in vec2 in_vert;
layout(location = 1) in vec3 in_color;
out vec3 v_color;    // Goes to the fragment shader

void main() {
    gl_Position = vec4(in_vert, 0.0, 1.0);
    v_color = in_color;
}
