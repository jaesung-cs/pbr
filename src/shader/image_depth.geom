#version 430 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec2 screen_size;
uniform vec4 xywh;

out vec2 tex_coord;

void main()
{
  vec2 offset = vec2(xywh.z / screen_size.x, - xywh.w / screen_size.y) * 2.f;

  gl_Position = gl_in[0].gl_Position;
  tex_coord = vec2(0.f, 0.f);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(offset.x, 0.f, 0.f, 0.f);
  tex_coord = vec2(1.f, 0.f);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(0.f, offset.y, 0.f, 0.f);
  tex_coord = vec2(0.f, 1.f);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(offset, 0.f, 0.f);
  tex_coord = vec2(1.f, 1.f);
  EmitVertex();
}
