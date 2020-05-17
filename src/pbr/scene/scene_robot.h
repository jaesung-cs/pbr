#ifndef PBR_SCENE_SCENE_ROBOT_H_
#define PBR_SCENE_SCENE_ROBOT_H_

#include "pbr/scene/scene_node.h"

#include "pbr/robot/robot_model.h"

namespace pbr
{
class SceneRobot : public SceneNode
{
public:
  SceneRobot() = delete;

  explicit SceneRobot(std::shared_ptr<RobotModel> robot_model);

  ~SceneRobot() override;

  bool IsRobot() const noexcept override { return true; }

  std::shared_ptr<SceneNode> Link(const std::string& link_name) const;

  void SetJointValue(const std::string& joint_name, double joint_value);

private:
  void GenerateMeshNodes(std::shared_ptr<SceneNode> scene_node, std::shared_ptr<RobotLink> link);

  std::shared_ptr<SceneNode> base_;
  std::unordered_map<std::string, std::shared_ptr<RobotJoint>> joint_name_to_joint_;
  std::unordered_map<std::string, std::shared_ptr<SceneNode>> joint_name_to_node_;
  std::unordered_map<std::string, std::shared_ptr<SceneNode>> link_name_to_node_;
};
}

#endif // PBR_SCENE_SCENE_ROBOT_H_
