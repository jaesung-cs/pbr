#include "pbr/render/gl/gl_pointcloud.h"

#include <iostream>

namespace pbr
{
namespace gl
{
Pointcloud::Pointcloud(int width, int height)
  : width_(width), height_(height)
{
}

Pointcloud::~Pointcloud()
{
  if (generated_)
    Clear();
}

void Pointcloud::Draw()
{
  if (!generated_)
    Generate();

  glBindVertexArray(vao_);
  glDrawArrays(GL_POINTS, 0, width_ * height_);
  glBindVertexArray(0);
}

void Pointcloud::Clear()
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

void Pointcloud::Generate()
{
  if (generated_)
    Clear();

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);

  using BaseType = unsigned short;

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferStorage(GL_ARRAY_BUFFER, sizeof(BaseType) * (width_ * height_) * 2, 0, GL_MAP_WRITE_BIT);

  auto ptr = (BaseType*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(BaseType) * (width_ * height_) * 2, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
  for (BaseType y = 0; y < height_; y++)
  {
    for (BaseType x = 0; x < width_; x++)
    {
      *(ptr++) = x;
      *(ptr++) = y;
    }
  }
  glUnmapBuffer(GL_ARRAY_BUFFER);

  glVertexAttribIPointer(0, 2, GL_UNSIGNED_SHORT, 0, (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  generated_ = true;
}
}
}
