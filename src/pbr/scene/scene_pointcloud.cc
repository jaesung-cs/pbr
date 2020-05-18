#include "pbr/scene/scene_pointcloud.h"

namespace pbr
{
ScenePointcloud::ScenePointcloud(std::shared_ptr<Kinect2> kinect, std::shared_ptr<ImageWidget> color_image_widget, std::shared_ptr<ImageWidget> depth_image_widget)
  : kinect_(kinect), color_image_widget_(color_image_widget), depth_image_widget_(depth_image_widget)
{
}

ScenePointcloud::~ScenePointcloud() = default;
}
