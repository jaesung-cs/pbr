#include "pbr/sensor/color_sensor.h"

namespace pbr
{
ColorSensor::ColorSensor() = default;

ColorSensor::~ColorSensor() = default;

bool ColorSensor::UpdateColorImage(Image& image)
{
  return false;
}

std::shared_ptr<Image> ColorSensor::CreateColorImage()
{
  return std::make_shared<Image>(ColorWidth(), ColorHeight(), 4, (unsigned char)0);
}
}
