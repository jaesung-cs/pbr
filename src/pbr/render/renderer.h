#ifndef PBR_RENDER_RENDERER_H_
#define PBR_RENDER_RENDERER_H_

#include <future>
#include <array>
#include <chrono>
#include <unordered_map>

#include "pbr/render/gl/gl_program.h"
#include "pbr/render/gl/gl_model.h"
#include "pbr/render/gl/gl_texture.h"
#include "pbr/render/gl/gl_axis.h"
#include "pbr/render/gl/gl_grid.h"
#include "pbr/render/gl/gl_image_point.h"
#include "pbr/render/gl/gl_pointcloud.h"
#include "pbr/render/camera.h"
#include "pbr/data/image.h"
#include "pbr/scene/scene.h"
#include "pbr/scene/material.h"
#include "pbr/widget/image_widget.h"

namespace pbr
{
class Renderer
{
public:
  Renderer();
  ~Renderer();

  void Resize(int width, int height);
  void MouseButton(int button, int action, int mods, double x, double y);
  void MouseMove(double x, double y);
  void Keyboard(int key, int action, int mods);

  void Initialize();
  void Draw();

  void SetWidget(std::shared_ptr<Widget> widget)
  {
    widget_ = widget;
  }

private:
  void DrawWidget(std::shared_ptr<Widget> widget, int x = 0, int y = 0);
  void DrawScene(std::shared_ptr<Scene> scene, int width, int height);
  void DrawImage(std::shared_ptr<ImageWidget> image_widget, int x, int y);

  int width_ = 1200;
  int height_ = 800;

  void DrawScene(std::shared_ptr<SceneNode> node, Affine3d transform = Affine3d::Identity());

  std::shared_ptr<Widget> widget_;
  Material material_;

  gl::Program program_model_{ "..\\src\\shader", "model" };
  gl::Program program_color_{ "..\\src\\shader", "color" };
  gl::Program program_image_color_{ "..\\src\\shader", "image_color" };
  gl::Program program_image_depth_{ "..\\src\\shader", "image_depth" };
  gl::Program program_pointcloud_{ "..\\src\\shader", "pointcloud" };

  std::unordered_map<std::string, gl::Model> models_;
  std::unordered_map<std::string, gl::Texture> textures_;
  std::unordered_map<std::shared_ptr<SceneNode>, gl::Pointcloud> pointclouds_;

  gl::Axis axis_;
  gl::Grid grid_;

  std::unordered_map<std::shared_ptr<ImageWidget>, gl::Texture> image_textures_;
  gl::ImagePoint image_point_;
};
}

#endif // PBR_RENDER_RENDERER_H_
