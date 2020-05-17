#include "pbr/render/gl/gl_grid.h"

namespace pbr
{
namespace gl
{
Grid::Grid()
{
}

Grid::~Grid()
{
  if (generated_)
    Clear();
}

void Grid::SetGridSize(int grid_size)
{
  grid_size_ = grid_size;
}

void Grid::Draw()
{
  if (!generated_)
    Generate();

  if (buffered_grid_size_ != grid_size_)
    UpdateBuffer();

  glBindVertexArray(vao_);
  glDrawArrays(GL_LINES, 0, NumVertices());
  glBindVertexArray(0);
}

void Grid::Clear()
{
  if (generated_)
  {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);

    vao_ = 0;
    vbo_ = 0;

    generated_ = false;
  }
}

void Grid::Generate()
{
  if (generated_)
    Clear();

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferStorage(GL_ARRAY_BUFFER, sizeof(float) * 1048576, 0, GL_MAP_WRITE_BIT);

  UpdateBuffer();

  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  generated_ = true;
}

void Grid::UpdateBuffer()
{
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  const auto num_vertices = NumVertices();
  float* ptr = (float*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(float) * num_vertices * 6, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
  for (int i = -grid_size_; i <= grid_size_; i++)
  {
    *(ptr++) = -grid_size_;
    *(ptr++) = static_cast<float>(i);
    *(ptr++) = 0.f;
    *(ptr++) = 0.8f;
    *(ptr++) = 0.8f;
    *(ptr++) = 0.8f;

    *(ptr++) = grid_size_;
    *(ptr++) = static_cast<float>(i);
    *(ptr++) = 0.f;
    *(ptr++) = 0.8f;
    *(ptr++) = 0.8f;
    *(ptr++) = 0.8f;
  }
  for (int i = -grid_size_; i <= grid_size_; i++)
  {
    *(ptr++) = static_cast<float>(i);
    *(ptr++) = -grid_size_;
    *(ptr++) = 0.f;
    *(ptr++) = 0.8f;
    *(ptr++) = 0.8f;
    *(ptr++) = 0.8f;

    *(ptr++) = static_cast<float>(i);
    *(ptr++) = grid_size_;
    *(ptr++) = 0.f;
    *(ptr++) = 0.8f;
    *(ptr++) = 0.8f;
    *(ptr++) = 0.8f;
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  buffered_grid_size_ = grid_size_;
}

int Grid::NumVertices()
{
  return (grid_size_ * 2 + 1) * 2 * 2;
}
}
}
