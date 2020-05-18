#ifndef PBR_RENDER_GL_GL_POINTCLOUD_CLIENT_H_
#define PBR_RENDER_GL_GL_POINTCLOUD_CLIENT_H_

#include <glad/glad.h>

#include "pbr/types.h"
#include "pbr/data/pointcloud.h"

namespace pbr
{
namespace gl
{
class PointcloudClient
{
public:
  PointcloudClient();
  ~PointcloudClient();

  void UpdatePointcloud(const pbr::Pointcloud& pointcloud);

  void Draw();

private:
  void Clear();
  void Generate();

  int num_points_ = 0;

  bool generated_ = false;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;

  int buffer_size_ = 0;
};
}
}

#endif // PBR_RENDER_GL_GL_POINTCLOUD_CLIENT_H_
