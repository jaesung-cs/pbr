#include "pbr/render/renderer.h"

#include <iostream>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "pbr/scene/scene_mesh.h"
#include "pbr/scene/scene_grid.h"
#include "pbr/scene/scene_pointcloud.h"

#include "pbr/widget/scene_widget.h"
#include "pbr/widget/image_widget.h"

namespace pbr
{
Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Resize(int width, int height)
{
  width_ = width;
  height_ = height;
}

void Renderer::MouseButton(int button, int action, int mods, double x, double y)
{
}

void Renderer::MouseMove(double x, double y)
{
}

void Renderer::Keyboard(int key, int action, int mods)
{
}

void Renderer::Initialize()
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);

  // Material initialization
  material_.Ambient() = Vector3f(0.5f, 0.5f, 0.5f);
  material_.Diffuse() = Vector3f(0.5f, 0.5f, 0.5f);
  material_.Specular() = Vector3f(0.5f, 0.5f, 0.5f);
  material_.Shininess() = 0.05f;
}

void Renderer::Draw()
{
  glViewport(0, 0, width_, height_);
  glClearColor(1.0f, 1.0f, 1.0f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  DrawWidget(widget_);
}

void Renderer::DrawWidget(std::shared_ptr<Widget> widget, int x, int y)
{
  x += widget->X();
  y += widget->Y();

  if (widget->IsScene())
  {
    glViewport(x, y, widget->Width(), widget->Height());
    auto scene_widget = std::dynamic_pointer_cast<SceneWidget>(widget);
    DrawScene(scene_widget->GetScene(), widget->Width(), widget->Height());
  }

  else if (widget->IsImage())
  {
    glViewport(0, 0, width_, height_);
    auto image_widget = std::dynamic_pointer_cast<ImageWidget>(widget);
    DrawImage(image_widget, x, y);
  }

  for (auto child : widget->Children())
  {
    DrawWidget(child);
  }
}

void Renderer::DrawImage(std::shared_ptr<ImageWidget> image_widget, int x, int y)
{
  auto& texture = image_textures_[image_widget];

  if (image_widget->ShouldUpdate())
  {
    texture.Update(*image_widget->Image());
    image_widget->FinishedUpdate();
  }

  bool valid_texture = true;
  switch (image_widget->Image()->GlType())
  {
  case GL_UNSIGNED_BYTE:
    program_image_color_.Use();
    program_image_color_.Uniform2f("screen_size", width_, height_);
    program_image_color_.Uniform4f("xywh", x, y, image_widget->Width(), image_widget->Height());
    break;

  case GL_UNSIGNED_SHORT:
    program_image_depth_.Use();
    program_image_depth_.Uniform2f("screen_size", width_, height_);
    program_image_depth_.Uniform4f("xywh", x, y, image_widget->Width(), image_widget->Height());
    break;

  default:
    valid_texture = false;
    break;
  }

  if (valid_texture)
  {
    texture.Bind();
    image_point_.Draw();
  }
}

void Renderer::DrawScene(std::shared_ptr<Scene> scene, int width, int height)
{
  auto camera = scene->ViewCamera();

  // Set up uniforms
  program_color_.Use();
  program_color_.UniformMatrix4f("projection", camera->Projection());
  program_color_.UniformMatrix4f("view", camera->View());

  program_pointcloud_.Use();
  program_pointcloud_.UniformMatrix4f("projection", camera->Projection());
  program_pointcloud_.UniformMatrix4f("view", camera->View());
  program_pointcloud_.Uniform2f("screen_size", width, height);
  program_pointcloud_.Uniform1i("color_texture", 0);
  program_pointcloud_.Uniform1i("depth_texture", 1);

  program_model_.Use();
  program_model_.UniformMatrix4f("projection", camera->Projection());
  program_model_.UniformMatrix4f("view", camera->View());
  program_model_.Uniform3f("eye", camera->Position());
  program_model_.Uniform1f("alpha", 1.f);

  // Light update to shader
  for (int i = 0; i < scene->NumLights(); i++)
  {
    const auto& light = scene->Light(i);
    std::string prefix = "lights[" + std::to_string(i) + "]";

    program_model_.Uniform1i(prefix + ".use", 1);

    if (light.IsDirectional())
      program_model_.Uniform1i(prefix + ".type", 0);
    else if (light.IsPoint())
      program_model_.Uniform1i(prefix + ".type", 1);

    program_model_.Uniform3f(prefix + ".position", light.Position());
    program_model_.Uniform3f(prefix + ".ambient", light.Ambient());
    program_model_.Uniform3f(prefix + ".diffuse", light.Diffuse());
    program_model_.Uniform3f(prefix + ".specular", light.Specular());
    program_model_.Uniform3f(prefix + ".attenuation", light.Attenuation());
  }
  for (int i = scene->NumLights(); i < 8; i++)
  {
    std::string prefix = "lights[" + std::to_string(i) + "]";
    program_model_.Uniform1i(prefix + ".use", 0);
  }

  // Material update
  program_model_.Uniform3f("material.ambient", material_.Ambient());
  program_model_.Uniform3f("material.diffuse", material_.Diffuse());
  program_model_.Uniform3f("material.specular", material_.Specular());
  program_model_.Uniform1f("material.shininess", material_.Shininess());

  DrawScene(scene->Root());
}

void Renderer::DrawScene(std::shared_ptr<SceneNode> node, Affine3d transform)
{
  transform = transform * node->Transform();

  if (node->IsMesh())
  {
    auto mesh = std::dynamic_pointer_cast<SceneMesh>(node);
    const auto& filename = mesh->Filename();

    if (models_.find(filename) == models_.cend())
      models_[filename].Load(filename, mesh->ShouldRecenter());

    auto& model = models_[filename];

    if (!mesh->DiffuseTextureFilename().empty())
      model.SetDiffuseTextureFilename(mesh->DiffuseTextureFilename());

    program_model_.Use();
    program_model_.UniformMatrix4f("model", transform.cast<float>().matrix());

    // Async texture load
    if (model.HasTexture())
    {
      const auto& texture_filename = model.DiffuseTextureFilename();
      if (textures_.find(texture_filename) == textures_.cend())
        textures_[texture_filename].Load(texture_filename);

      auto& texture = textures_[texture_filename];

      program_model_.Uniform1i("has_diffuse_texture", 1);
      program_model_.Uniform1i("diffuse_texture", 0);
      program_model_.Uniform1f("tex_coord_scale", mesh->TexCoordScale());
      texture.Bind();
    }
    else
    {
      program_model_.Uniform1i("has_diffuse_texture", 0);
    }

    models_[filename].Draw();
  }

  else if (node->IsGrid())
  {
    auto grid = std::dynamic_pointer_cast<SceneGrid>(node);

    int num_grid = grid->NumGrid();
    double interval = grid->Interval();

    Affine3d scale = Affine3d::Identity();
    scale.scale(interval);

    program_color_.Use();
    program_color_.UniformMatrix4f("model", (transform * scale).cast<float>().matrix());
    grid_.SetGridSize(num_grid);
    grid_.Draw();
  }

  else if (node->IsAxis())
  {
    program_color_.Use();
    program_color_.UniformMatrix4f("model", transform.cast<float>().matrix());
    axis_.Draw();
  }

  else if (node->IsRobot())
  {
  }

  else if (node->IsPointcloud())
  {
    auto pointcloud_node = std::dynamic_pointer_cast<ScenePointcloud>(node);
    
    auto kinect = pointcloud_node->Kinect();
    const auto& color_param = kinect->ColorCameraParam();
    const auto& depth_param = kinect->DepthCameraParam();

    // Uniform variables
    program_pointcloud_.Use();

    // TODO: upload uniform variables once on initialization
    program_pointcloud_.Uniform4f("color_camera_param", color_param.Fx(), color_param.Fy(), color_param.Cx(), color_param.Cy());
    program_pointcloud_.Uniform4f("depth_camera_param", depth_param.Fx(), depth_param.Fy(), depth_param.Cx(), depth_param.Cy());
    program_pointcloud_.UniformMatrix3f("camera_rotation", kinect->CameraRotation());
    program_pointcloud_.Uniform3f("camera_translation", kinect->CameraTranslation());

    program_pointcloud_.UniformMatrix4f("model", transform.cast<float>().matrix());

    // TODO: bind color image to TEXTURE0
    auto color_image_widget = pointcloud_node->ColorImageWidget();
    auto& color_texture = image_textures_[color_image_widget];
    color_texture.Bind();

    // TODO: bind depth image to TEXTURE1
    glActiveTexture(GL_TEXTURE1);
    auto depth_image_widget = pointcloud_node->DepthImageWidget();
    auto& depth_texture = image_textures_[depth_image_widget];
    depth_texture.Bind();

    glActiveTexture(GL_TEXTURE0);

    pointcloud_.Draw();
  }

  for (auto child : node->Children())
    DrawScene(child, transform);
}
}
