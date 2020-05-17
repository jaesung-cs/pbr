#version 430 core
in vec2 tex_coord;

uniform usampler2D tex;

out vec4 out_color;

void main()
{
  uint depth = texture(tex, tex_coord).r;
  float gray = float(depth) / 5000.f;
  out_color = vec4(vec3(gray), 1.f);
}
