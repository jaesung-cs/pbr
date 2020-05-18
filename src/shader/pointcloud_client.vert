#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec2 screen_size;

out Vertex
{
  vec3 color;
  float width;
  float height;
  float depth;
} frag;

void main()
{
  vec4 m = model * vec4(position, 1.f);
  vec4 vm = view * m;
  gl_Position = projection * vm;

  frag.color = color;
  frag.depth = position.z;
  
  frag.width = (frag.depth * 2.f / screen_size.x) * 2.0;
  frag.height = (frag.depth * 2.f / screen_size.y) * 2.0;

  /*
  frag.width = (-vm.z * 2.f / screen_size.x) * 1.3;
  frag.height = (-vm.z * 2.f / screen_size.y) * 1.3;
  */
}
