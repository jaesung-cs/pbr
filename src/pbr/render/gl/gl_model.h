#ifndef PBR_RENDER_GL_GL_MODEL_H_
#define PBR_RENDER_GL_GL_MODEL_H_

#include <string>
#include <vector>

#include <glad/glad.h>

#include "pbr/types.h"

namespace pbr
{
namespace gl
{
class Model
{
public:
  Model();
  ~Model();

  void Load(const std::string& filename, bool recenter = false);

  bool HasTexture() const
  {
    return has_diffuse_texture_;
  }

  void SetDiffuseTextureFilename(const std::string& diffuse_texture_filename)
  {
    has_diffuse_texture_ = !diffuse_texture_filename.empty();
    diffuse_texture_filename_ = diffuse_texture_filename;
  }

  const auto& DiffuseTextureFilename() const noexcept { return diffuse_texture_filename_; }

  void Draw();

private:
  void Clear();

  std::string filename_;

  bool generated_ = false;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
  GLuint ibo_ = 0;

  int num_vertices_ = 0;
  int num_elements_ = 0;

  bool has_diffuse_texture_ = false;
  std::string diffuse_texture_filename_;
};
}
}

#endif // PBR_RENDER_GL_GL_MODEL_H_
