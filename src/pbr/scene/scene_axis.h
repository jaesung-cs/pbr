#ifndef PBR_SCENE_SCENE_AXIS_H_
#define PBR_SCENE_SCENE_AXIS_H_

#include "pbr/scene/scene_node.h"

namespace pbr
{
class SceneAxis : public SceneNode
{
public:
  SceneAxis();
  ~SceneAxis() override;

  bool IsAxis() const noexcept override { return true; }

private:
};
}

#endif // PBR_SCENE_SCENE_AXIS_H_
