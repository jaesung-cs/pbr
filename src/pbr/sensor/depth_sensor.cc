#include "pbr/sensor/depth_sensor.h"

namespace pbr
{
DepthSensor::DepthSensor() = default;

DepthSensor::~DepthSensor() = default;

bool DepthSensor::UpdateDepthImage(Image& image)
{
  return false;
}

Image DepthSensor::CreateDepthImage()
{
  return Image(DepthWidth(), DepthHeight(), 1, (unsigned short)300);
}
}
