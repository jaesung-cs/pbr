#include "pbr/sensor/depth_sensor.h"

namespace pbr
{
DepthSensor::DepthSensor() = default;

DepthSensor::~DepthSensor() = default;

bool DepthSensor::UpdateDepthImage(Image& image)
{
  return false;
}

std::shared_ptr<Image> DepthSensor::CreateDepthImage()
{
  return std::make_shared<Image>(DepthWidth(), DepthHeight(), 1, (unsigned short)0);
}
}
