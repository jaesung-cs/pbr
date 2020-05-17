#ifndef PBR_WIDGET_SCENE_WIDGET_H_
#define PBR_WIDGET_SCENE_WIDGET_H_

#include "pbr/widget/widget.h"

#include "pbr/scene/scene.h"

namespace pbr
{
class SceneWidget : public Widget
{
public:
  SceneWidget();
  SceneWidget(int width, int height);
  SceneWidget(int x, int y, int width, int height);

  ~SceneWidget() override;

  bool IsScene() const noexcept override { return true; }

  void SetScene(std::shared_ptr<Scene> scene) { scene_ = scene; }
  auto GetScene() { return scene_; }

private:
  std::shared_ptr<Scene> scene_;
};
}

#endif // PBR_WIDGET_SCENE_WIDGET_H_
