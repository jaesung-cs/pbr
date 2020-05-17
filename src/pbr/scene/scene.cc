#include "pbr/scene/scene.h"

namespace pbr
{
Scene::Scene()
{
  root_ = std::make_shared<SceneNode>();
}

Scene::~Scene() = default;
}
