#version 330 core
#ifdef VERTEX_SHADER
layout(location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * vec4(position, 1.0);
}
#endif
#ifdef FRAGMENT_SHADER
uniform vec4 color;
out vec4 frag_color;

void main()
{
  frag_color = color;
}
#endif