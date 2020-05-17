#ifndef PBR_SCENE_MATERIAL_H_
#define PBR_SCENE_MATERIAL_H_

#include "pbr/types.h"

namespace pbr
{
class Material
{
public:
  Material() = default;

  ~Material() = default;

  auto& Ambient() { return ambient_; }
  const auto& Ambient() const { return ambient_; }

  auto& Diffuse() { return diffuse_; }
  const auto& Diffuse() const { return diffuse_; }

  auto& Specular() { return specular_; }
  const auto& Specular() const { return specular_; }

  auto& Alpha() { return alpha_; }
  const auto& Alpha() const { return alpha_; }

  auto& Shininess() { return shininess_; }
  const auto& Shininess() const { return shininess_; }

private:
  Vector3f ambient_{ 1.f, 1.f, 1.f };
  Vector3f diffuse_{ 1.f, 1.f, 1.f };
  Vector3f specular_{ 1.f, 1.f, 1.f };
  float alpha_;
  float shininess_;
};
}

#endif // PBR_SCENE_MATERIAL_H_
