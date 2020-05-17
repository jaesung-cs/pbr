#include "pbr/render/gl/gl_image_point.h"

namespace pbr
{
namespace gl
{
ImagePoint::ImagePoint()
{
}

ImagePoint::~ImagePoint()
{
  if (generated_)
    Clear();
}

void ImagePoint::Draw()
{
  if (!generated_)
    Generate();

  glBindVertexArray(vao_);
  glDrawArrays(GL_POINTS, 0, 1);
  glBindVertexArray(0);
}

void ImagePoint::Clear()
{
  if (generated_)
  {
    glDeleteVertexArrays(1, &vao_);
    
    vao_ = 0;

    generated_ = false;
  }
}

void ImagePoint::Generate()
{
  if (generated_)
    Clear();

  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  glVertexAttrib2f(0, 0.f, 0.f);

  glBindVertexArray(0);

  generated_ = true;
}
}
}
