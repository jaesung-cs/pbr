#ifndef PBR_SCENE_SCENE_POINTCLOUD_H_
#define PBR_SCENE_SCENE_POINTCLOUD_H_

#include "pbr/scene/scene_node.h"

#include "pbr/widget/image_widget.h"

#include "pbr/data/pointcloud.h"
#include "pbr/sensor/kinect2.h"

namespace pbr
{
class ScenePointcloud : public SceneNode
{
public:
  ScenePointcloud() = delete;

  // TODO: change image widgets to just images
  ScenePointcloud(std::shared_ptr<Kinect2> kinect, std::shared_ptr<ImageWidget> color_image_widget, std::shared_ptr<ImageWidget> depth_image_widget);

  ~ScenePointcloud() override;

  bool IsPointcloud() const noexcept override { return true; }

  const auto& Pointcloud() const { return pointcloud_; }
  auto& Pointcloud() { return pointcloud_; }

  auto Kinect() const { return kinect_; }

  // TODO: change image widgets to just images
  auto ColorImageWidget() const { return color_image_widget_; }
  auto DepthImageWidget() const { return depth_image_widget_; }

private:
  pbr::Pointcloud pointcloud_;
  std::shared_ptr<Kinect2> kinect_;
  std::shared_ptr<ImageWidget> color_image_widget_;
  std::shared_ptr<ImageWidget> depth_image_widget_;
};
}

#endif // PBR_SCENE_SCENE_POINTCLOUD_H_
