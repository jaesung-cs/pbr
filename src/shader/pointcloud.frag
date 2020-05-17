#version 430 core
in vec3 frag_color;
in float frag_depth;

out vec4 out_color;

void main()
{
  /*
  // Fragments over 2 meter range are discarded
  if (frag_depth >= 2.f)
    discard;
    */

  out_color = vec4(frag_color, 1.f);
}
