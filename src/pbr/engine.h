#ifndef PBR_ENGINE_H_
#define PBR_ENGINE_H_

#include <vector>
#include <random>

#include "pbr/render/renderer.h"
#include "pbr/scene/scene.h"
#include "pbr/widget/scene_widget.h"
#include "pbr/widget/image_widget.h"
#include "pbr/robot/robot_model.h"
#include "pbr/scene/scene_robot.h"
#include "pbr/scene/scene_pointcloud.h"
#include "pbr/data/dataset_wnp.h"
#include "pbr/sensor/kinect2.h"

struct GLFWwindow;

namespace pbr
{
class Engine
{
private:
  using Clock = std::chrono::high_resolution_clock;
  using Timestamp = std::chrono::time_point<Clock, std::chrono::duration<double>>;
  using Duration = std::chrono::duration<double>;

public:
  Engine();
  ~Engine();

  // Callbacks
  void Resize(int width, int height);
  void MouseButton(int button, int action, int mods, double x, double y);
  void MouseMove(double x, double y);
  void Keyboard(int key, int action, int mods);

  // Main loop
  void Run();

  void Initialize();

  // Data handling
  void RandomizeRobotJoints();

private:
  int width_ = 1600;
  int height_ = 800;

  GLFWwindow* window_ = 0;

  Renderer renderer_;
  std::shared_ptr<Scene> scene_;

  std::shared_ptr<Widget> screen_widget_;
  std::shared_ptr<SceneWidget> scene_widget_;
  std::shared_ptr<ImageWidget> color_image_widget_;
  std::shared_ptr<ImageWidget> depth_image_widget_;

  std::shared_ptr<RobotModel> robot_model_;
  std::shared_ptr<SceneRobot> robot_node_;
  std::shared_ptr<ScenePointcloud> pointcloud_node_;

  std::shared_ptr<Dataset> dataset_;
  std::shared_ptr<DatasetWnp> dataset_wnp_;

  // Kinect sensor
  std::shared_ptr<Kinect2> kinect_;
  std::shared_ptr<ImageWidget> kinect_color_image_widget_;
  std::shared_ptr<ImageWidget> kinect_depth_image_widget_;

  // Random number generator
  std::random_device random_device_;
  std::mt19937 generator_;
  std::uniform_real_distribution<> uniform_distribution_{ 0., 1. };

  // Mouse
  std::array<bool, 3> mouse_buttons_{ false, false, false };
  double mouse_last_x_ = 0.;
  double mouse_last_y_ = 0.;

  // Camera movement
  std::shared_ptr<Camera> camera_;
  std::shared_ptr<Camera> headmount_camera_;
  std::array<bool, 6> move_key_pressed_{ false, false, false, false, false, false };
  double camera_move_speed_ = 1.0;

  // Animation
  bool animation_ = false;
  Timestamp animation_start_time_;
  Duration animation_time_;
};
}

#endif // PBR_ENGINE_H_
