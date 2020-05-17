#include "pbr/render/camera.h"

#include <iostream>

namespace pbr
{
Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Translate(double xp, double yp)
{
  Vector3f x = up_.cross(-direction_).normalized();
  Vector3f y = -direction_.cross(x).normalized();

  position_ += xp * x;
  position_ += yp * y;
}

void Camera::TranslateUpward(double zp)
{
  position_ += zp * up_;
}

void Camera::TranslateForward(double zp)
{
  Vector3f z = direction_;

  position_ += zp * z;
}

void Camera::Zoom(double yp)
{
  float new_fov;
  switch (projection_type_)
  {
  case ProjectionType::PERSPECTIVE:
    new_fov = perspective_fov_ + yp;
    if (new_fov < 10.f * (PI / 180.f))
      perspective_fov_ = 10.f * (PI / 180.f);
    else if (new_fov > 170.f * (PI / 180.f))
      perspective_fov_ = 170.f * (PI / 180.f);
    else
      perspective_fov_ = new_fov;
    break;

  case ProjectionType::ORTHO:
    new_fov = ortho_fov_ + yp;
    if (new_fov < 0.1f)
      ortho_fov_ = 0.1f;
    else if (new_fov > 10.f)
      ortho_fov_ = 10.f;
    else
      ortho_fov_ = new_fov;
    break;
  }
}

void Camera::Rotate(double xp, double yp)
{
  float x = xp;
  float y = yp;

  constexpr float min_angle = 0.5f;

  Eigen::Matrix3f m;
  m.col(1) = direction_.cross(up_).normalized();
  m.col(0) = m.col(1).cross(up_).normalized();
  m.col(2) = m.col(0).cross(m.col(1));

  float current_yangle = acosf(up_.dot(-direction_));
  if (y < 0 && current_yangle + y < min_angle)
    y = min_angle - current_yangle;
  else if (y > 0 && current_yangle + y > PI - min_angle)
    y = PI - min_angle - current_yangle;

  Matrix3f r1 = AngleAxisf(y, Eigen::Vector3f(0.f, 1.f, 0.f)).matrix();
  Matrix3f r2 = AngleAxisf(x, Eigen::Vector3f(0.f, 0.f, 1.f)).matrix();

  direction_ = m * r2 * r1 * m.transpose() * direction_;
}

void Camera::TranslateOnScreen(double xp, double yp)
{
  Translate(-xp * sensitivity_translation_, yp * sensitivity_translation_);
}

void Camera::TranslateForwardOnScreen(double zp)
{
  TranslateForward(-zp * sensitivity_translation_);
}

void Camera::ZoomOnScreen(double xp, double yp)
{
  Zoom(yp);
}

void Camera::RotateOnScreen(double xp, double yp)
{
  Rotate(-xp * sensitivity_rotation_, -yp * sensitivity_rotation_);
}

Matrix4f Camera::Projection() const
{
  switch (projection_type_)
  {
  case ProjectionType::PERSPECTIVE:
    return Perspective();

  case ProjectionType::ORTHO:
    return Ortho();

  default:
    return Matrix4f::Identity();
  }
}

Matrix4f Camera::Ortho() const
{
  float width = ortho_fov_ * aspect_;
  float height = ortho_fov_;

  Eigen::Matrix4f p;

  p.setIdentity();
  p(0, 0) = 2.f / width;
  p(1, 1) = 2.f / height;
  p(2, 2) = -2.f / (far_ - near_);
  p(2, 3) = -(far_ + near_) / (far_ - near_);

  return p;
}

Matrix4f Camera::Perspective() const
{
  Eigen::Matrix4f p;

  p.setIdentity();
  p(1, 1) = 1.f / tanf(perspective_fov_ / 2.f);
  p(0, 0) = p(1, 1) / aspect_;
  p(2, 2) = -(far_ + near_) / (far_ - near_);
  p(2, 3) = -2.f * far_ * near_ / (far_ - near_);
  p(3, 2) = -1.f;
  p(3, 3) = 0.f;

  return p;
}

Matrix4f Camera::View() const
{
  Vector3f x = up_.cross(-direction_).normalized();
  Vector3f y = -direction_.cross(x).normalized();
  Vector3f z = x.cross(y);

  Matrix4f m;
  m.setIdentity();
  m.block(0, 0, 1, 3) = x.transpose();
  m.block(1, 0, 1, 3) = y.transpose();
  m.block(2, 0, 1, 3) = z.transpose();

  Matrix4f t;
  t.setIdentity();
  t.block(0, 3, 3, 1) = -position_;

  return m * t;
}
}
