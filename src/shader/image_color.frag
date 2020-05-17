#version 430 core
in vec2 tex_coord;

uniform sampler2D tex;

out vec4 out_color;

void main()
{
  out_color = vec4(texture(tex, tex_coord).rgb, 1.f);
}
