#ifndef PBR_TYPES_H_
#define PBR_TYPES_H_

#include <Eigen/Dense>

namespace pbr
{
template <int dim>
using Vectorf = Eigen::Matrix<float, dim, 1>;

using Vector2i = Eigen::Vector2i;

using Vector2f = Eigen::Vector2f;
using Vector3f = Eigen::Vector3f;
using Vector4f = Eigen::Vector4f;

using Vector2d = Eigen::Vector2d;
using Vector3d = Eigen::Vector3d;
using Vector4d = Eigen::Vector4d;

using Matrix2f = Eigen::Matrix2f;
using Matrix3f = Eigen::Matrix3f;
using Matrix4f = Eigen::Matrix4f;

using Matrix2d = Eigen::Matrix2d;
using Matrix3d = Eigen::Matrix3d;
using Matrix4d = Eigen::Matrix4d;

using AngleAxisf = Eigen::AngleAxisf;

using AngleAxisd = Eigen::AngleAxisd;

using Affine3f = Eigen::Affine3f;

using Affine3d = Eigen::Affine3d;
}

#endif // PBR_TYPES_H_
