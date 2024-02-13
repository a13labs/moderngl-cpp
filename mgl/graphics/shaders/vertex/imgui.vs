#version 330

layout(location = 0) in vec2 i_position;
layout(location = 1) in vec2 i_uv;
layout(location = 2) in vec4 i_color;

out vec2 f_uv;
out vec4 f_color;

uniform mat4 projection;

void main() {
    f_uv = i_uv;
    f_color = i_color;
    gl_Position = projection * vec4(i_position.xy, 0, 1);
}
