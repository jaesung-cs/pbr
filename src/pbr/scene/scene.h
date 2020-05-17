#ifndef PBR_SCENE_SCENE_H_
#define PBR_SCENE_SCENE_H_

#include <vector>

#include "pbr/scene/light.h"
#include "pbr/scene/scene_node.h"
#include "pbr/render/camera.h"

namespace pbr
{
class Scene
{
public:
  Scene();
  ~Scene();

  void AddLight(const Light& light) { lights_.push_back(light); }
  void AddLight(Light&& light) { lights_.emplace_back(std::move(light)); }

  int NumLights() const { return lights_.size(); }

  auto& Light(int idx) { return lights_[idx]; }
  const auto& Light(int idx) const { return lights_[idx]; }

  auto Root() const { return root_; }

  void AddCamera(std::shared_ptr<Camera> camera)
  {
    cameras_.push_back(camera);

    if (view_camera_ == nullptr)
      view_camera_ = camera;
  }

  auto Camera(int idx) const { return cameras_[idx]; }
  void SetViewCamera(int idx) { view_camera_ = cameras_[idx]; }
  auto ViewCamera() const { return view_camera_; }

private:
  std::vector<pbr::Light> lights_;
  std::shared_ptr<SceneNode> root_;
  std::vector<std::shared_ptr<pbr::Camera>> cameras_;
  std::shared_ptr<pbr::Camera> view_camera_;
};
}

#endif // PBR_SCENE_SCENE_H_
