#ifndef PBR_RENDER_CAMERA_H_
#define PBR_RENDER_CAMERA_H_

#include "pbr/types.h"

namespace pbr
{
class Camera
{
private:
  static constexpr float PI = 3.1415926535897932384626433832795f;

public:
  enum class ProjectionType
  {
    PERSPECTIVE,
    ORTHO,
  };

public:
  Camera();
  ~Camera();

  void SetAspect(float aspect)
  {
    aspect_ = aspect;
  }

  void SetFov(float fov)
  {
    perspective_fov_ = fov;
  }

  void SetOrthoFov(float zoom)
  {
    ortho_fov_ = zoom;
  }

  const auto& Position() const noexcept { return position_; }

  void SetPosition(const Eigen::Vector3f& position) { position_ = position; }

  void SetPosition(float x, float y, float z)
  {
    position_.x() = x;
    position_.y() = y;
    position_.z() = z;
  }

  void SetDirection(const Vector3f& dir)
  {
    direction_ = dir.normalized();
  }

  void Translate(double xp, double yp);
  void TranslateUpward(double zp);
  void TranslateForward(double zp);
  void Zoom(double yp);
  void Rotate(double xp, double yp);

  void TranslateOnScreen(double xp, double yp);
  void TranslateForwardOnScreen(double zp);
  void ZoomOnScreen(double xp, double yp);
  void RotateOnScreen(double xp, double yp);

  Matrix4f Projection() const;
  Matrix4f Ortho() const;
  Matrix4f Perspective() const;

  Matrix4f View() const;

private:
  ProjectionType projection_type_ = ProjectionType::PERSPECTIVE;
  Vector3f position_{ 0.f, 0.f, 0.f };
  Vector3f direction_{ 0.f, -1.f, 0.f };
  Vector3f up_{ 0.f, 0.f, 1.f };
  float near_ = 0.1f;
  float far_ = 10.f;
  float aspect_ = 4.f / 3.f;
  float perspective_fov_ = 60.f * (PI / 180.f);
  float ortho_fov_ = 1.f;

  float sensitivity_translation_ = 0.002f;
  float sensitivity_zoom_ = 0.002f;
  float sensitivity_rotation_ = 0.002f;
};
}

#endif // PBR_RENDER_CAMERA_H_
