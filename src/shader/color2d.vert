#version 430 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 color;

out vec3 frag_color;

void main()
{
  gl_Position = vec4(pos, 0.f, 1.f);
  frag_color = color;
}
