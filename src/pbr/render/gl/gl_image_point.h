#ifndef PBR_RENDER_GL_GL_IMAGE_POINT_H_
#define PBR_RENDER_GL_GL_IMAGE_POINT_H_

#include <string>
#include <vector>

#include <glad/glad.h>

#include "pbr/types.h"

namespace pbr
{
namespace gl
{
class ImagePoint
{
public:
  ImagePoint();
  ~ImagePoint();

  void Draw();

private:
  void Clear();
  void Generate();

  bool generated_ = false;
  GLuint vao_ = 0;
};
}
}

#endif // PBR_RENDER_GL_GL_IMAGE_POINT_H_
