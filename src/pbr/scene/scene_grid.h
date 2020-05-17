#ifndef PBR_SCENE_SCENE_GRID_H_
#define PBR_SCENE_SCENE_GRID_H_

#include "pbr/scene/scene_node.h"

namespace pbr
{
class SceneGrid : public SceneNode
{
public:
  SceneGrid();

  SceneGrid(int num_grid, double interval);

  ~SceneGrid() override;

  bool IsGrid() const noexcept override { return true; }

  auto Interval() const { return interval_; }

  void SetInterval(double interval);

  auto NumGrid() const { return num_grid_; }

  void SetNumGrid(int num_grid);

private:
  int num_grid_ = 1;
  double interval_ = 1.;
};
}

#endif // PBR_SCENE_SCENE_GRID_H_
