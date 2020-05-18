#include "pbr/engine.h"

#include <iostream>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "pbr/scene/scene_axis.h"
#include "pbr/scene/scene_grid.h"
#include "pbr/scene/scene_mesh.h"
#include "pbr/robot/robot_model_loader.h"

namespace pbr
{
namespace
{
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  auto engine = (Engine*)glfwGetWindowUserPointer(window);
  engine->Resize(width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  double x, y;
  glfwGetCursorPos(window, &x, &y);

  auto engine = (Engine*)glfwGetWindowUserPointer(window);
  engine->MouseButton(button, action, mods, x, y);
}

void cursor_pos_callback(GLFWwindow* window, double x, double y)
{
  auto engine = (Engine*)glfwGetWindowUserPointer(window);
  engine->MouseMove(x, y);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  auto engine = (Engine*)glfwGetWindowUserPointer(window);
  engine->Keyboard(key, action, mods);
}
}

Engine::Engine()
{
  generator_.seed(random_device_());

  Initialize();
}

Engine::~Engine()
{
}

void Engine::Resize(int width, int height)
{
  width_ = width;
  height_ = height;

  // Resize widget size
  screen_widget_->Resize(width, height);

  renderer_.Resize(width, height);
}

void Engine::MouseButton(int button, int action, int mods, double x, double y)
{
  bool is_pressed = action == GLFW_PRESS;
  int button_index = button == GLFW_MOUSE_BUTTON_LEFT ? 0 : button == GLFW_MOUSE_BUTTON_MIDDLE ? 1 : button == GLFW_MOUSE_BUTTON_RIGHT ? 2 : -1;

  if (button_index >= 0 && button_index < 3)
    mouse_buttons_[button_index] = is_pressed;

  mouse_last_x_ = x;
  mouse_last_y_ = y;
}

void Engine::MouseMove(double x, double y)
{
  double dx = x - mouse_last_x_;
  double dy = y - mouse_last_y_;

  // Move camera when the scene is viewed from main camera
  if (scene_->ViewCamera() == camera_)
  {
    // Left + right click: zoom
    if (mouse_buttons_[0] && mouse_buttons_[2])
    {
      camera_->TranslateForwardOnScreen(dy);
    }

    // Left click: rotation
    else if (mouse_buttons_[0])
    {
      camera_->RotateOnScreen(dx, dy);
    }

    // Right click: translation
    else if (mouse_buttons_[2])
    {
      camera_->TranslateOnScreen(dx, dy);
    }
  }

  mouse_last_x_ = x;
  mouse_last_y_ = y;
}

void Engine::Keyboard(int key, int action, int mods)
{
  if (action == GLFW_PRESS || action == GLFW_REPEAT)
  {
    switch (key)
    {
      // Randomize robot joint values
    case GLFW_KEY_R:
      RandomizeRobotJoints();
      break;
    }
  }

  // Select camera
  if (action == GLFW_PRESS)
  {
    bool other_key_pressed = false;
    for (auto pressed : mouse_buttons_)
      other_key_pressed |= pressed;
    for (auto pressed : move_key_pressed_)
      other_key_pressed |= pressed;

    if (!other_key_pressed)
    {
      switch (key)
      {
        // View from free camera
      case GLFW_KEY_1:
        scene_->SetViewCamera(0);
        break;

        // View from headmount camera
      case GLFW_KEY_2:
        scene_->SetViewCamera(1);
        break;

        // Change sequence
      case GLFW_KEY_UP:
        dataset_->PreviousSequence();
        break;

        // Change sequence
      case GLFW_KEY_DOWN:
        dataset_->NextSequence();
        break;

        // Restart animation
      case GLFW_KEY_ENTER:
        dataset_->Rewind();
        break;
      }
    }
  }
  if (action == GLFW_PRESS || action == GLFW_RELEASE)
  {
    switch (key)
    {
      // Movement
    case GLFW_KEY_W:
      move_key_pressed_[0] = action == GLFW_PRESS;
      break;

    case GLFW_KEY_S:
      move_key_pressed_[1] = action == GLFW_PRESS;
      break;

    case GLFW_KEY_A:
      move_key_pressed_[2] = action == GLFW_PRESS;
      break;

    case GLFW_KEY_D:
      move_key_pressed_[3] = action == GLFW_PRESS;
      break;

    case GLFW_KEY_SPACE:
      move_key_pressed_[4] = action == GLFW_PRESS;
      break;
    }
  }
}

void Engine::RandomizeRobotJoints()
{
  for (const auto& joint : robot_model_->GetJoints())
  {
    auto lower = joint->GetLower();
    auto upper = joint->GetUpper();
    auto t = uniform_distribution_(generator_);
    auto joint_value = lower * (1. - t) + upper * t;

    robot_node_->SetJointValue(joint->GetName(), joint_value);
  }
}

void Engine::Initialize()
{
  // Scene initialization
  scene_ = std::make_shared<Scene>();

  // Cameras
  camera_ = std::make_shared<Camera>();
  camera_->SetPosition(-1.f, 2.f, 2.f);
  camera_->SetDirection(Vector3f(1.f, -1.f, -1.f).normalized());
  scene_->AddCamera(camera_);

  headmount_camera_ = std::make_shared<Camera>();
  scene_->AddCamera(headmount_camera_);

  // Axis
  auto root = scene_->Root();
  auto axis_node = std::make_shared<SceneAxis>();
  root->AddChild(axis_node);
  axis_node->SetParent(root);

  // Grid
  auto grid_node = std::make_shared<SceneGrid>(5, 1);
  root->AddChild(grid_node);
  grid_node->SetParent(root);

  // Lights
  Light light;

  light.SetDirectional();
  light.Position() = Vector3f(0.f, 0.f, 1.f);
  light.Ambient() = Vector3f(0.7f, 0.7f, 0.7f);
  light.Diffuse() = Vector3f(0.2f, 0.2f, 0.2f);
  light.Specular() = Vector3f(0.2f, 0.2f, 0.2f);
  scene_->AddLight(std::move(light));

  // Robot model
  RobotModelLoader robot_model_loader;
  robot_model_loader.SetPackageDirectory("..\\..\\fetch_ros");
  robot_model_ = robot_model_loader.Load("package://fetch_description\\robots\\fetch.urdf");

  // Robot node
  robot_node_ = std::make_shared<SceneRobot>(robot_model_);
  root->AddChild(robot_node_);
  robot_node_->SetParent(root);

  // Bookshelf
  constexpr double pi = 3.1415926535897932384626433832795;
  /*
  auto shelf_node = std::make_shared<SceneMesh>("..\\data\\shelf2\\Estanteria.dae");
  shelf_node->DiffuseTextureFilename() = "..\\data\\shelf2\\1.png";
  shelf_node->Transform().translate(Vector3d(0.8, 0.5, 0));
  shelf_node->Transform().rotate(AngleAxisd(-60. * pi / 180., Vector3d(0., 0., 1.)));
  shelf_node->Transform().translate(Vector3d(0., 0., 0.78));
  shelf_node->Transform().scale(0.01);
  shelf_node->Recenter();
  shelf_node->TexCoordScale() = 0.01;
  root->AddChild(shelf_node);
  shelf_node->SetParent(root);
  */

  // Screen widget initialization
  screen_widget_ = std::make_shared<Widget>(1620, 800);

  // Dataset load
  dataset_wnp_ = std::make_shared<DatasetWnp>("..\\..\\dataset\\watch-n-patch");
  //dataset_wnp_ = std::make_shared<DatasetWnp>("..\\..\\dataset\\watchnpatch\\tools");
  dataset_ = dataset_wnp_;

  // Dataset images
  color_image_widget_ = std::make_shared<ImageWidget>(0, 0, 400, 200);
  screen_widget_->AddChild(color_image_widget_);
  color_image_widget_->SetParent(screen_widget_);

  depth_image_widget_ = std::make_shared<ImageWidget>(0, 200, 400, 200);
  screen_widget_->AddChild(depth_image_widget_);
  depth_image_widget_->SetParent(screen_widget_);

  // Scene widget is initialized here, after defining two image widgets holding Kinect images
  scene_widget_ = std::make_shared<SceneWidget>(400, 0, 1200, 800);
  scene_widget_->SetScene(scene_);
  screen_widget_->AddChild(scene_widget_);
  scene_widget_->SetParent(screen_widget_);

  // Kinect images
  kinect_ = std::make_shared<Kinect2>();

  // Live Kinect sensor images
  kinect_color_image_widget_ = std::make_shared<ImageWidget>(0, 400, 400, 200);
  kinect_color_image_widget_->UpdateImage(kinect_->CreateColorImage());
  screen_widget_->AddChild(kinect_color_image_widget_);
  kinect_color_image_widget_->SetParent(screen_widget_);

  kinect_depth_image_widget_ = std::make_shared<ImageWidget>(0, 600, 400, 200);
  kinect_depth_image_widget_->UpdateImage(kinect_->CreateDepthImage());
  screen_widget_->AddChild(kinect_depth_image_widget_);
  kinect_depth_image_widget_->SetParent(screen_widget_);

  // Pointcloud from images
  pointcloud_node_ = std::make_shared<ScenePointcloud>(kinect_, color_image_widget_, depth_image_widget_); // Dataset images
  //pointcloud_node_ = std::make_shared<ScenePointcloud>(kinect_, kinect_color_image_widget_, kinect_depth_image_widget_); // Live kinect images

  // Attach pointcloud node to robot camera
  //pointcloud_node_->Transform().translate(Vector3d(0.2, 0., 1.));
  pointcloud_node_->Transform().rotate(AngleAxisd(90. * pi / 180., Vector3d(0., 1., 0.)));
  pointcloud_node_->Transform().rotate(AngleAxisd(-90. * pi / 180., Vector3d(0., 0., 1.)));
  auto robot_camera_node = robot_node_->Link("head_camera_link");
  robot_camera_node->AddChild(pointcloud_node_);
  pointcloud_node_->SetParent(robot_camera_node);
}

void Engine::Run()
{
  // GLFW init
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  // GLFW create window
  window_ = glfwCreateWindow(width_, height_, "pbr", NULL, NULL);
  if (window_ == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window_);

  // GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }

  glfwSetWindowUserPointer(window_, this);
  glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
  glfwSetKeyCallback(window_, key_callback);
  glfwSetMouseButtonCallback(window_, mouse_button_callback);
  glfwSetCursorPosCallback(window_, cursor_pos_callback);


  // Initialize for drawing
  renderer_.Resize(width_, height_);

  renderer_.Initialize();

  renderer_.SetWidget(screen_widget_);


  // Resize window before running main loop
  Resize(width_, height_);

  // Main loop
  using Clock = std::chrono::system_clock;
  using Timestamp = Clock::time_point;
  using Duration = std::chrono::duration<double>;

  constexpr double fps = 60.;
  auto frame_length = Duration(1. / fps);
  Timestamp first_timestamp = Clock::now();
  Timestamp last_update_time = Clock::now();
  Timestamp camera_last_timestamp = Clock::now();

  int frame_count = 0;

  // Pointcloud
  Pointcloud pointcloud;

  while (!glfwWindowShouldClose(window_))
  {
    // Update events
    glfwPollEvents();

    Timestamp current_time = Clock::now();

    // Set headmount camera position
    auto robot_camera_node = robot_node_->Link("head_camera_link");
    auto headmount_camera_transform = robot_camera_node->TransformFromRoot();
    auto headmount_camera_transform_matrix = headmount_camera_transform.matrix().cast<float>();
    headmount_camera_->SetDirection(headmount_camera_transform_matrix.block(0, 0, 3, 1));
    headmount_camera_->SetPosition(headmount_camera_transform_matrix.block(0, 3, 3, 1));

    // Update scene

    // Kinect scene update
    if (kinect_->Update(*kinect_color_image_widget_->Image(), *kinect_depth_image_widget_->Image(), pointcloud_node_->Pointcloud()))
    {
      kinect_color_image_widget_->NeedUpdate();
      kinect_depth_image_widget_->NeedUpdate();
      //pointcloud_node_->NeedUpdate();
    }

    // Load images from dataset
    color_image_widget_->UpdateImage(dataset_->GetColorImage());
    depth_image_widget_->UpdateImage(dataset_->GetDepthImage());

    // Generate point cloud using Kinect v2 mapper
    /*
    if (kinect_->GeneratePointcloudWithCoordinateMapper(dataset_->GetColorImage(), dataset_->GetDepthImage(), pointcloud_node_->Pointcloud()))
    {
      pointcloud_node_->NeedUpdate();
      dataset_->NextFrame();
    }
    */

    // Generate point cloud using camera parameters
    /*
    if (kinect_->GeneratePointcloudWithCameraParameters(dataset_->GetColorImage(), dataset_->GetDepthImage(), pointcloud_node_->Pointcloud()))
    {
      pointcloud_node_->NeedUpdate();
      dataset_->NextFrame();
    }
    */

    // Draw pointcloud without creating pointcloud
    //pointcloud_node_->NeedUpdate();
    dataset_->NextFrame();

    // Draw scene
    renderer_.Draw();

    // Sleep until next update
    /*
    auto duration = current_time - last_update_time;

    if (duration < frame_length)
      std::this_thread::sleep_for(frame_length - duration);
      */
    //using namespace std::chrono_literals;
    //std::this_thread::sleep_for(10ms);

    last_update_time = Clock::now();

    // Buffer swap
    glfwSwapBuffers(window_);

    // DEBUG: print fps
    /*
    frame_count++;
    auto running_time = Duration(current_time - first_timestamp).count();

    std::cout << frame_count / running_time << std::endl;
    */

    // Move camera when the scene is viewed from main camera
    Duration camera_duration = current_time - camera_last_timestamp;
    if (scene_->ViewCamera() == camera_)
    {
      // Update camera
      if (move_key_pressed_[0])
        camera_->TranslateForward(camera_move_speed_ * camera_duration.count());
      if (move_key_pressed_[1])
        camera_->TranslateForward(-camera_move_speed_ * camera_duration.count());
      if (move_key_pressed_[2])
        camera_->Translate(-camera_move_speed_ * camera_duration.count(), 0.);
      if (move_key_pressed_[3])
        camera_->Translate(camera_move_speed_ * camera_duration.count(), 0.);
      if (move_key_pressed_[4])
        camera_->TranslateUpward(camera_move_speed_ * camera_duration.count());
    }
    camera_last_timestamp = current_time;
  }

  glfwTerminate();
}
}
