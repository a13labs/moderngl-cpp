#version 330 core
layout (location = 0) in vec2 i_position; 
layout (location = 1) in vec2 i_uv; 

out vec2 f_uv;

uniform mat4 projection; // ortho matrix

void main()
{
  gl_Position = projection  * vec4(i_position.xy, 0.0, 1.0);
  f_uv = i_uv;
}

