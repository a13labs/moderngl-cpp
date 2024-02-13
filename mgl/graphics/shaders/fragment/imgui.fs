#version 330

in vec2 f_uv;
in vec4 f_color;

uniform sampler2D tex;

out vec4 o_color;

void main() {
    o_color = f_color * texture(tex, f_uv.st);
}
