#version 430 core
layout (location = 0) in vec2 position;

uniform vec2 screen_size;
uniform vec4 xywh;

void main()
{
  gl_Position = vec4((xywh.x / screen_size.x * 2.f) - 1.f, ((1.f - xywh.y / screen_size.y) * 2.f) - 1.f, 0.f, 1.f);
}
