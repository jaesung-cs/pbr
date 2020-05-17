#include "pbr/scene/scene_grid.h"

namespace pbr
{
SceneGrid::SceneGrid() = default;

SceneGrid::SceneGrid(int num_grid, double interval)
{
  SetNumGrid(num_grid);
  SetInterval(interval);
}

SceneGrid::~SceneGrid() = default;

void SceneGrid::SetInterval(double interval)
{
  constexpr double minimum_interval = 1e-6;

  if (interval <= minimum_interval)
    interval_ = minimum_interval;
  else
    interval_ = interval;
}

void SceneGrid::SetNumGrid(int num_grid)
{
  if (num_grid <= 1)
    num_grid_ = 1;
  else
    num_grid_ = num_grid;
}
}
