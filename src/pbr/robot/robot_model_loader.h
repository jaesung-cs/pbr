#ifndef PBR_ROBOT_ROBOT_MODEL_LOADER_H_
#define PBR_ROBOT_ROBOT_MODEL_LOADER_H_

#include <string>
#include <vector>
#include <memory>

#include "pbr/robot/robot_model.h"

#include <tinyxml2/tinyxml2.h>

namespace pbr
{
class RobotModelLoader
{
public:
  RobotModelLoader();
  ~RobotModelLoader();

  void SetPackageDirectory(const std::string& package_directory);
  std::shared_ptr<RobotModel> Load(const std::string& filename);

private:
  RobotLink::Geometry GetGeometryFromElement(tinyxml2::XMLElement* element);

  std::string ConvertFilenamePackageDirectory(const std::string& filename);

  std::string package_directory_;
};
}

#endif // PBR_ROBOT_ROBOT_MODEL_LOADER_H_
