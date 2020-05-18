#include "pbr/data/pointcloud.h"

namespace pbr
{
Pointcloud::Pointcloud() = default;

Pointcloud::~Pointcloud() = default;

void Pointcloud::Resize(int num_points)
{
  num_points_ = num_points;
  buffer_.resize(num_points * 6, 0.f);
}
}
