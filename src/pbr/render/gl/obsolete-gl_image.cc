#include "pbr/render/gl/gl_image.h"

#include <iostream>

namespace pbr
{
namespace gl
{
Image::Image(int width, int height, int num_components)
  : width_(width), height_(height), num_components_(num_components)
{
}

Image::~Image()
{
  if (generated_)
    Clear();
}

void Image::Update(unsigned char* data)
{
  if (!generated_)
    Generate();

  glBindTexture(GL_TEXTURE_2D, tex_);
  if (num_components_ == 3)
  {
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, data);
  }
  else if (num_components_ == 4)
  {
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, data);
  }

  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Image::Update(const pbr::Image& image)
{
  if (!generated_)
    Generate();

  if (num_components_ == 3 && image.NumComponents() == 3)
  {
    glBindTexture(GL_TEXTURE_2D, tex_);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, image.Data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  else if (num_components_ == 4 && image.NumComponents() == 4)
  {
    glBindTexture(GL_TEXTURE_2D, tex_);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, image.Data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void Image::Draw()
{
  if (!generated_)
    Generate();

  std::cout << tex_ << std::endl;

  glBindVertexArray(vao_);
  glBindTexture(GL_TEXTURE_2D, tex_);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Image::Clear()
{
  if (generated_)
  {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteTextures(1, &tex_);

    vao_ = 0;
    vbo_ = 0;
    tex_ = 0;

    generated_ = false;
  }
}

void Image::Generate()
{
  if (generated_)
    Clear();

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);

  constexpr float buffer[] = {
    0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
  };

  glBufferStorage(GL_ARRAY_BUFFER, sizeof(float) * 16, buffer, 0);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate texture
  glGenTextures(1, &tex_);
  glBindTexture(GL_TEXTURE_2D, tex_);

  // Set the texture wrapping/filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

  if (num_components_ == 3)
  {
    glTexStorage2D(GL_TEXTURE_2D, 3, GL_RGB8, width_, height_);
  }
  else if (num_components_ == 4)
  {
    glTexStorage2D(GL_TEXTURE_2D, 3, GL_RGBA8, width_, height_);
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  generated_ = true;
}
}
}
