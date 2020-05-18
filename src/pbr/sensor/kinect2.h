#ifndef PBR_SENSOR_KINECT2_H_
#define PBR_SENSOR_KINECT2_H_

#include "pbr/sensor/color_sensor.h"
#include "pbr/sensor/depth_sensor.h"

#include <Kinect.h>

#include "pbr/data/pointcloud.h"
#include "pbr/sensor/camera_param.h"

namespace pbr
{
class Kinect2 : public ColorSensor, public DepthSensor
{
public:
  Kinect2();
  ~Kinect2();

  void Open();

  int ColorWidth() const override { return 1920; }
  int ColorHeight() const override { return 1080; }
  int DepthWidth() const override { return 512; }
  int DepthHeight() const override { return 424; }

  // Separate update of color and depth is disabled
  bool UpdateColorImage(Image& image) override;
  bool UpdateDepthImage(Image& image) override;

  bool Update(Image& color_image, Image& depth_image, Pointcloud& pointcloud);

  bool GeneratePointcloudWithCoordinateMapper(const Image& color_image, const Image& depth_image, Pointcloud& pointcloud);
  bool GeneratePointcloudWithCameraParameters(const Image& color_image, const Image& depth_image, Pointcloud& pointcloud);

  // Camera parameters
  const auto& ColorCameraParam() const { return color_param_; }
  const auto& DepthCameraParam() const { return depth_param_; }
  const auto& CameraRotation() const { return rotation_; }
  const auto& CameraTranslation() const { return translation_; }

private:
  bool opened_ = false;
  IKinectSensor* sensor_ = nullptr;
  IMultiSourceFrameReader* reader_ = nullptr;

  //The coordinateMapper function in the sdk for mapping the color frameand depth frame uses the camera calibration parameters.
  // The default camera calibration parameters are had coded in the sdk, however there are slight differences in eachand every device.
  // You could try to recalibrate the Kinect camerasand use the updated calibration parameters to get the correct overlay of the colorand depth maps.
  // Note however, that by siply rplacing the camera calibration parameters in the Kinect Fusion codeand recompiling does not work,
  // as the parameters are replaced from the closed - source Kinect fusion dll.So you'll have to write your own code to update each frame at runtime.
  ICoordinateMapper* mapper_ = nullptr;

  // Instead of using mapper, use camera parameters to calibrate
  CameraParam color_param_{ 1050.60702, 1052.77088, 983.72076 - 1., 530.46047 - 1., 0.02468, -0.02250, -0.00338, 0.00482, 0.00000 }; // Shift by (-1, -1) to account the starting index of matlab and c++
  CameraParam depth_param_{ 365.19599, 365.61839, 266.84507 - 1., 199.14226 - 1., 0.11926, -0.53343, -0.00021, 0.00730, 0.00000 }; // Shift by (-1, -1) to account the starting index of matlab and c++
  Matrix3f rotation_;
  Vector3f translation_{ -56.7170118956704 / 1000.f, -0.728510987903765 / 1000.f, -3.87867708001932 / 1000.f }; // Convert millimeter to meter
  //Vector3f translation_{ 70. / 1000.f, -0. / 1000.f, -0. / 1000.f }; // Convert millimeter to meter

  // TODO: size to const numbers
  ColorSpacePoint depth_to_rgb_[512 * 424] = { 0.f, };     // Maps depth pixels to rgb pixels
  CameraSpacePoint depth_to_xyz_[512 * 424] = { 0.f, };    // Maps depth pixels to 3d coordinates
};
}

#endif // PBR_SENSOR_KINECT2_H_
