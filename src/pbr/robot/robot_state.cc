#include "pbr/robot/robot_state.h"

namespace pbr
{
RobotState::RobotState(std::shared_ptr<RobotModel> robot_model)
  : robot_model_(robot_model)
{
}

RobotState::~RobotState() = default;
}
