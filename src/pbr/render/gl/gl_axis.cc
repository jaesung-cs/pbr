#include "pbr/render/gl/gl_axis.h"

namespace pbr
{
namespace gl
{
Axis::Axis()
{
}

Axis::~Axis()
{
  if (generated_)
    Clear();
}

void Axis::Draw()
{
  if (!generated_)
    Generate();

  glBindVertexArray(vao_);
  glDrawArrays(GL_LINES, 0, 6);
  glBindVertexArray(0);
}

void Axis::Clear()
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

void Axis::Generate()
{
  if (generated_)
    Clear();

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);

  constexpr float buffer[] = {
    0.0f, 0.0f, 0.f, 1.f, 0.f, 0.f,
    1.0f, 0.0f, 0.f, 1.f, 0.f, 0.f,
    0.0f, 0.0f, 0.f, 0.f, 1.f, 0.f,
    0.0f, 1.0f, 0.f, 0.f, 1.f, 0.f,
    0.0f, 0.0f, 0.f, 0.f, 0.f, 1.f,
    0.0f, 0.0f, 1.f, 0.f, 0.f, 1.f,
  };

  glBufferStorage(GL_ARRAY_BUFFER, sizeof(float) * 36, buffer, 0);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  generated_ = true;
}
}
}
