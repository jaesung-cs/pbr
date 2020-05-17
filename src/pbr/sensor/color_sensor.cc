#include "pbr/sensor/color_sensor.h"

namespace pbr
{
ColorSensor::ColorSensor() = default;

ColorSensor::~ColorSensor() = default;

bool ColorSensor::UpdateColorImage(Image& image)
{
  return false;
}

Image ColorSensor::CreateColorImage()
{
  return Image(ColorWidth(), ColorHeight(), 4, (unsigned char)0);
}
}
