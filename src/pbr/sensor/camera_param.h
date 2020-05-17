#ifndef PBR_SENSOR_CAMERA_PARAM_H_
#define PBR_SENSOR_CAMERA_PARAM_H_

#include "pbr/types.h"

namespace pbr
{
class CameraParam
{
public:
  CameraParam() = default;

  CameraParam(float fx, float fy, float cx, float cy, float k1, float k2, float p1, float p2, float k3)
    : f_(fx, fy), c_(cx, cy), k_(k1, k2, k3), p_(p1, p2)
  {
  }

  ~CameraParam() = default;

  auto Fx() const { return f_(0); }
  auto Fy() const { return f_(1); }
  auto Cx() const { return c_(0); }
  auto Cy() const { return c_(1); }
  auto K1() const { return k_(0); }
  auto K2() const { return k_(1); }
  auto K3() const { return k_(2); }
  auto P1() const { return p_(0); }
  auto P2() const { return p_(1); }

  auto& Fx() { return f_(0); }
  auto& Fy() { return f_(1); }
  auto& Cx() { return c_(0); }
  auto& Cy() { return c_(1); }
  auto& K1() { return k_(0); }
  auto& K2() { return k_(1); }
  auto& K3() { return k_(2); }
  auto& P1() { return p_(0); }
  auto& P2() { return p_(1); }

  auto F() const { return f_; }
  auto C() const { return c_; }
  auto K() const { return k_; }
  auto P() const { return p_; }

  auto& F() { return f_; }
  auto& C() { return c_; }
  auto& K() { return k_; }
  auto& P() { return p_; }

private:
  Vector2f f_;
  Vector2f c_;
  Vector3f k_;
  Vector2f p_;
};
}

#endif // PBR_SENSOR_CAMERA_PARAM_H_
