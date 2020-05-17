#include "pbr/scene/scene_robot.h"

#include "pbr/scene/scene_mesh.h"

namespace pbr
{
SceneRobot::SceneRobot(std::shared_ptr<RobotModel> robot_model)
{
  base_ = std::make_shared<SceneNode>();

  AddChild(base_);

  // TODO: set base_'s parent node to shared_ptr of this

  GenerateMeshNodes(base_, robot_model->GetBaseLink());
}

SceneRobot::~SceneRobot() = default;

void SceneRobot::GenerateMeshNodes(std::shared_ptr<SceneNode> scene_node, std::shared_ptr<RobotLink> link)
{
  for (const auto& visual : link->GetVisuals())
  {
    auto visual_node = std::make_shared<SceneMesh>(visual.geometry.mesh_filename);
    visual_node->Transform() = visual.origin;

    scene_node->AddChild(visual_node);
    visual_node->SetParent(scene_node);
  }

  for (auto child_joint : link->GetChildJoints())
  {
    auto child_node = std::make_shared<SceneNode>();
    child_node->Transform() = child_joint->GetTransform(0.);

    scene_node->AddChild(child_node);
    child_node->SetParent(scene_node);

    joint_name_to_joint_[child_joint->GetName()] = child_joint;
    joint_name_to_node_[child_joint->GetName()] = child_node;

    auto child_link = child_joint->GetChildLink();

    link_name_to_node_[child_link->GetName()] = child_node;
    GenerateMeshNodes(child_node, child_link);
  }
}

void SceneRobot::SetJointValue(const std::string& joint_name, double joint_value)
{
  joint_name_to_node_[joint_name]->Transform() = joint_name_to_joint_[joint_name]->GetTransform(joint_value);
}

std::shared_ptr<SceneNode> SceneRobot::Link(const std::string& link_name) const
{
  const auto it = link_name_to_node_.find(link_name);
  if (it == link_name_to_node_.cend())
    return nullptr;
  return it->second;
}
}
