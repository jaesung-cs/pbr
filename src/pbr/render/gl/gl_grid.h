#ifndef PBR_RENDER_GL_GL_GRID_H_
#define PBR_RENDER_GL_GL_GRID_H_

#include <glad/glad.h>

#include "pbr/types.h"

namespace pbr
{
namespace gl
{
class Grid
{
public:
  Grid();
  ~Grid();

  void SetGridSize(int grid_size);

  void Draw();

private:
  void Clear();
  void Generate();
  void UpdateBuffer();
  int NumVertices();

  int grid_size_ = 1;
  int buffered_grid_size_ = 0;

  bool generated_ = false;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;

  int buffer_size_ = 0;
};
}
}

#endif // PBR_RENDER_GL_GL_GRID_H_
