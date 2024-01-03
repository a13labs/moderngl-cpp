/**
 * @file   color.fs
 * @brief  A fragment shader that outputs a uniform color.
*/
#version 330 core
uniform vec4 color;
out vec4 frag_color;

void main()
{
  frag_color = color;
}
