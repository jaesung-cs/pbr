#ifndef PBR_RENDER_GL_GL_POINTCLOUD_H_
#define PBR_RENDER_GL_GL_POINTCLOUD_H_

#include <glad/glad.h>

#include "pbr/types.h"
#include "pbr/data/pointcloud.h"

namespace pbr
{
namespace gl
{
class Pointcloud
{
public:
  Pointcloud() = delete;

  Pointcloud(int depth_width, int depth_height);

  ~Pointcloud();

  void Draw();

private:
  void Clear();
  void Generate();

  int width_ = 0;
  int height_ = 0;

  bool generated_ = false;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
};
}
}

#endif // PBR_RENDER_GL_GL_POINTCLOUD_H_
