#include "pbr/scene/scene_node.h"

namespace pbr
{
SceneNode::SceneNode() = default;

SceneNode::~SceneNode() = default;

Affine3d SceneNode::TransformFromRoot() const
{
  auto parent = parent_.lock();
  if (parent)
    return parent->TransformFromRoot() * transform_;
  return transform_;
}
}
