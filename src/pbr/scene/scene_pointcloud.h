#ifndef PBR_SCENE_SCENE_POINTCLOUD_H_
#define PBR_SCENE_SCENE_POINTCLOUD_H_

#include "pbr/scene/scene_node.h"

#include "pbr/data/pointcloud.h"

namespace pbr
{
class ScenePointcloud : public SceneNode
{
public:
  ScenePointcloud();
  ~ScenePointcloud() override;

  bool IsPointcloud() const noexcept override { return true; }

  const auto& Pointcloud() const { return pointcloud_; }
  auto& Pointcloud() { return pointcloud_; }

private:
  pbr::Pointcloud pointcloud_;
};
}

#endif // PBR_SCENE_SCENE_POINTCLOUD_H_
