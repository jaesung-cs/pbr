#include "pbr/widget/scene_widget.h"

namespace pbr
{
SceneWidget::SceneWidget()
{
}

SceneWidget::SceneWidget(int width, int height)
  : Widget(width, height)
{
}

SceneWidget::SceneWidget(int x, int y, int width, int height)
  : Widget(x, y, width, height)
{
}

SceneWidget::~SceneWidget()
{
}
}
