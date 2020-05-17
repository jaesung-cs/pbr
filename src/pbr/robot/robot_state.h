#ifndef PBR_ROBOT_ROBOT_STATE_H_
#define PBR_ROBOT_ROBOT_STATE_H_

#include <memory>

#include "pbr/robot/robot_model.h"

namespace pbr
{
class RobotState
{
public:
  RobotState() = delete;

  explicit RobotState(std::shared_ptr<RobotModel> robot_model);

  ~RobotState();

private:
  const std::shared_ptr<RobotModel> robot_model_;
};
}

#endif // PBR_ROBOT_ROBOT_STATE_H_
