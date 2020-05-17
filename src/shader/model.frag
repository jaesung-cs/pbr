#version 430 core
struct Light
{
  bool use;
  int type;
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 attenuation;
};

struct Material
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_tex_coord;

const int NUM_LIGHTS = 8;

uniform Light lights[NUM_LIGHTS];
uniform Material material;
uniform vec3 eye;

uniform bool has_diffuse_texture;
uniform sampler2D diffuse_texture;

uniform float alpha;

out vec4 out_color;

vec3 calc_directional_light(Light light, vec3 material_ambient, vec3 material_diffuse, vec3 n, vec3 v)
{
  vec3 l = normalize(light.position);
  float diff = max(dot(n, l), 0.f);

  vec3 r = reflect(-l, n);
  float spec = pow(max(dot(v, r), 0.f), material.shininess);
  
  vec3 ambient = light.ambient * material_ambient;
  vec3 diffuse = diff * light.diffuse * material_diffuse;
  vec3 specular = spec * (light.specular * material.specular);

  return ambient + diffuse;// + specular;
}

vec3 calc_point_light(Light light, vec3 material_ambient, vec3 material_diffuse, vec3 n, vec3 v)
{
  vec3 l = normalize(light.position - frag_position);
  float diff = max(dot(n, l), 0.f);

  vec3 r = reflect(-l, n);
  float spec = pow(max(dot(v, r), 0.f), material.shininess);
  
  float d = length(light.position - frag_position);
  float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * d + light.attenuation.z * (d * d));

  vec3 ambient = light.ambient * material_ambient;
  vec3 diffuse = diff * light.diffuse * material_diffuse;
  vec3 specular = spec * light.specular * material.specular;

  return attenuation * (ambient + diffuse + specular);
}

void main()
{
  vec3 n = normalize(frag_normal);
  vec3 v = normalize(eye - frag_position);

  vec3 light_color = vec3(0.f, 0.f, 0.f);

  vec3 material_ambient;
  vec3 material_diffuse;
  if (has_diffuse_texture)
  {
    material_diffuse = vec3(texture(diffuse_texture, frag_tex_coord));
    material_ambient = material_diffuse;
  }
  else
  {
    material_ambient = material.ambient;
    material_diffuse = material.diffuse;
  }

  for (int i = 0; i < NUM_LIGHTS; i++)
  {
    if (lights[i].use)
	  {
  	  if (lights[i].type == 0)
	      light_color += calc_directional_light(lights[i], material_ambient, material_diffuse, n, v);

	    else if (lights[i].type == 1)
  	    light_color += calc_point_light(lights[i], material_ambient, material_diffuse, n, v);
	  }
  }

  out_color = vec4(light_color, 1.f);
}
