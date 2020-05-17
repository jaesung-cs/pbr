#ifndef PBR_SCENE_SCENE_MESH_H_
#define PBR_SCENE_SCENE_MESH_H_

#include <string>

#include "pbr/scene/scene_node.h"

namespace pbr
{
class SceneMesh : public SceneNode
{
public:
  SceneMesh() = delete;

  explicit SceneMesh(const std::string& filename);

  ~SceneMesh() override;

  bool IsMesh() const noexcept override { return true; }

  auto& Filename() { return filename_; }
  auto& DiffuseTextureFilename() { return diffuse_texture_filename_; }
  auto& TexCoordScale() { return tex_coord_scale_; }

  void Recenter()
  {
    recenter_ = true;
  }

  bool ShouldRecenter()
  {
    return recenter_;
  }

private:
  std::string filename_;

  double tex_coord_scale_ = 1.;
  bool recenter_ = false;
  std::string diffuse_texture_filename_;
};
}

#endif // PBR_SCENE_SCENE_NODE_H_
