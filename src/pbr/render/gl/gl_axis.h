#ifndef PBR_RENDER_GL_GL_AXIS_H_
#define PBR_RENDER_GL_GL_AXIS_H_

#include <string>
#include <vector>

#include <glad/glad.h>

#include "pbr/types.h"

namespace pbr
{
namespace gl
{
class Axis
{
public:
  Axis();
  ~Axis();

  void Draw();

private:
  void Clear();
  void Generate();

  bool generated_ = false;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
};
}
}

#endif // PBR_RENDER_GL_GL_AXIS_H_
