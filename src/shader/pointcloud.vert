#version 430 core
layout (location = 0) in ivec2 position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// params: fx, fy, cx, cy
uniform vec4 color_camera_param;
uniform vec4 depth_camera_param;
uniform mat3 camera_rotation;
uniform vec3 camera_translation;

uniform vec2 screen_size;

uniform sampler2D color_texture;
uniform usampler2D depth_texture;

out Vertex
{
  vec3 color;
  float width;
  float height;
  float depth;
} frag;

void main()
{
  //float depth = float(texelFetch(depth_texture, textureSize(depth_texture, 0) - position - 1, 0).r) / 1000.f; // Flip x and y coordinate
  //float depth = float(texelFetch(depth_texture, ivec2(position.x, textureSize(depth_texture, 0).y - position.y - 1), 0).r) / 1000.f; // Flip y coordinate
  float depth = float(texelFetch(depth_texture, position, 0).r) / 1000.f;

  vec3 depth_space = vec3((position - depth_camera_param.zw) * depth / depth_camera_param.xy, depth);

  gl_Position = projection * view * model * vec4(depth_space, 1.f);

  frag.depth = depth;
  frag.width = (frag.depth * 2.f / screen_size.x) * 1.5f;
  frag.height = (frag.depth * 2.f / screen_size.y) * 1.5f;


  vec3 world_space = camera_rotation * depth_space + camera_translation;
  ivec2 color_space = ivec2(round((world_space.xy * color_camera_param.xy) / world_space.z + color_camera_param.zw));

  // Check inside box
  vec2 s = step(0, color_space) - step(textureSize(color_texture, 0), color_space);
  if (s.x * s.y == 0.f)
  {
    // Outside color texture image
    frag.color = vec3(0.5f);
  }
  else
  {
    // Inside color texture image
    //frag.color = texelFetch(color_texture, textureSize(color_texture, 0) - color_space - 1, 0).rgb; // Flip x and y coordinate
    frag.color = texelFetch(color_texture, color_space, 0).rgb;
  }
}
