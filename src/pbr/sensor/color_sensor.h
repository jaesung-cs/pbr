#ifndef PBR_SENSOR_COLOR_SENSOR_H_
#define PBR_SENSOR_COLOR_SENSOR_H_

#include "pbr/data/image.h"

namespace pbr
{
class ColorSensor
{
public:
  ColorSensor();
  virtual ~ColorSensor();

  virtual int ColorWidth() const { return 1; }
  virtual int ColorHeight() const { return 1; }

  virtual bool UpdateColorImage(Image& image);

  std::shared_ptr<Image> CreateColorImage();

private:
};
}

#endif // PBR_SENSOR_COLOR_SENSOR_H_
