#include "pbr/render/gl/gl_pointcloud.h"

#include <iostream>

namespace pbr
{
namespace gl
{
Pointcloud::Pointcloud()
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

  if (num_points_ > 0)
  {
    glBindVertexArray(vao_);
    glDrawArrays(GL_POINTS, 0, num_points_);
    glBindVertexArray(0);
  }
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

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferStorage(GL_ARRAY_BUFFER, sizeof(float) * 2097152, 0, GL_MAP_WRITE_BIT);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  generated_ = true;
}

void Pointcloud::UpdatePointcloud(const pbr::Pointcloud& pointcloud)
{
  if (!generated_)
    Generate();

  num_points_ = pointcloud.NumPoints();

  if (num_points_ > 0)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    float* ptr = (float*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(float) * pointcloud.NumPoints() * 6, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
    std::memcpy(ptr, pointcloud.Buffer().data(), sizeof(float) * pointcloud.NumPoints() * 6);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  }
}
}
}
