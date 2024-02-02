#version 330 core

in vec2 texcoord;
out vec4 frag_color;

uniform sampler2D atlas;
// uniform vec4 color;

void main()
{
  vec4 texel = vec4(1.0, 1.0, 1.0, texture(atlas, texcoord).r);
  frag_color = vec4( 1.0 ) * texel;
}
