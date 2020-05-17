#ifndef PBR_SCENE_LIGHT_H_
#define PBR_SCENE_LIGHT_H_

#include "pbr/types.h"

namespace pbr
{
class Light
{
public:
  enum class Type
  {
    DIRECTIONAL = 0,
    POINT = 1,
  };

public:
  Light() = default;

  explicit Light(Type type)
    : type_(type)
  {
  }

  ~Light() = default;

  void SetDirectional() noexcept
  {
    type_ = Type::DIRECTIONAL;
  }

  void SetPoint() noexcept
  {
    type_ = Type::POINT;
  }

  bool IsDirectional() const noexcept
  {
    return type_ == Type::DIRECTIONAL;
  }

  bool IsPoint() const noexcept
  {
    return type_ == Type::POINT;
  }

  auto& Position() noexcept { return position_; }
  const auto& Position() const noexcept { return position_; }

  auto& Ambient() noexcept { return ambient_; }
  const auto& Ambient() const noexcept { return ambient_; }

  auto& Diffuse() noexcept { return diffuse_; }
  const auto& Diffuse() const noexcept { return diffuse_; }

  auto& Specular() noexcept { return specular_; }
  const auto& Specular() const noexcept { return specular_; }

  auto& Attenuation() noexcept { return attenuation_; }
  const auto& Attenuation() const noexcept { return attenuation_; }

private:
  Type type_ = Type::DIRECTIONAL;

  Vector3f position_{ 0.f, 0.f, 0.f };
  Vector3f ambient_{ 1.f, 1.f, 1.f };
  Vector3f diffuse_{ 1.f, 1.f, 1.f };
  Vector3f specular_{ 1.f, 1.f, 1.f };
  Vector3f attenuation_{ 1.f, 1.f, 1.f };
};
}

#endif // PBR_SCENE_LIGHT_H_
