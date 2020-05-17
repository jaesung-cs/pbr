#ifndef PBR_SCENE_SCENE_NODE_H_
#define PBR_SCENE_SCENE_NODE_H_

#include <memory>
#include <vector>

#include "pbr/types.h"

namespace pbr
{
class SceneNode
{
public:
  SceneNode();
  virtual ~SceneNode();

  virtual bool IsMesh() const noexcept { return false; }
  virtual bool IsPointcloud() const noexcept { return false; }
  virtual bool IsAxis() const noexcept { return false; }
  virtual bool IsGrid() const noexcept { return false; }
  virtual bool IsRobot() const noexcept { return false; }
  virtual bool IsImage() const noexcept { return false; }

  std::shared_ptr<SceneNode> Parent() const
  {
    return parent_.lock();
  }

  bool HasParent() const
  {
    return Parent() != nullptr;
  }

  void SetParent(std::shared_ptr<SceneNode> parent)
  {
    parent_ = parent;
  }

  void AddChild(std::shared_ptr<SceneNode> child)
  {
    children_.push_back(child);
  }

  int NumChild()
  {
    return children_.size();
  }

  auto Child(int idx) const
  {
    return children_[idx];
  }

  const auto& Children() const
  {
    return children_;
  }

  const auto& Transform() const
  {
    return transform_;
  }

  auto& Transform()
  {
    return transform_;
  }

  void NeedUpdate() noexcept
  {
    update_ = true;
  }

  bool ShouldUpdate() const noexcept
  {
    return update_;
  }

  void FinishedUpdate() noexcept
  {
    update_ = false;
  }

  Affine3d TransformFromRoot() const;

private:
  Affine3d transform_ = Affine3d::Identity();

  std::weak_ptr<SceneNode> parent_;
  std::vector<std::shared_ptr<SceneNode>> children_;

  bool update_ = false;
};
}

#endif // PBR_SCENE_SCENE_NODE_H_
