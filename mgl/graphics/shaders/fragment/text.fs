#version 330 core

in vec2 f_uv;
out vec4 o_color;

uniform sampler2D atlas;
uniform vec4 color;
uniform float px_range;

float screen_px_range() {
    vec2 unit_range = vec2(px_range)/vec2(textureSize(atlas, 0));
    vec2 screen_tex_size = vec2(1.0)/fwidth(f_uv);
    return max(0.5*dot(unit_range, screen_tex_size), 1.0);
}

void main()
{
  float sdf = texture(atlas, f_uv).r;
  float screen_px_distance = screen_px_range()*(sdf - 0.5);
  float alpha = clamp(screen_px_distance + 0.5, 0.0, 1.0);
  o_color = vec4(color.rgb, alpha*color.a);
}
