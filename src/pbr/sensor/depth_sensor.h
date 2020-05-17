#ifndef PBR_SENSOR_DEPTH_SENSOR_H_
#define PBR_SENSOR_DEPTH_SENSOR_H_

#include "pbr/data/image.h"

namespace pbr
{
class DepthSensor
{
public:
  DepthSensor();
  virtual ~DepthSensor();

  virtual int DepthWidth() const { return 1; }
  virtual int DepthHeight() const { return 1; }

  virtual bool UpdateDepthImage(Image& image);

  Image CreateDepthImage();

private:
};
}

#endif // PBR_SENSOR_DEPTH_SENSOR_H_
