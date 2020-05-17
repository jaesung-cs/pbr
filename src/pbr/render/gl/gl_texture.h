#ifndef PBR_RENDER_GL_GL_TEXTURE_H_
#define PBR_RENDER_GL_GL_TEXTURE_H_

#include <string>
#include <future>
#include <iostream>

#include <glad/glad.h>

#include "pbr/data/image.h"

namespace pbr
{
namespace gl
{
class Texture
{
public:
  Texture() = default;

  ~Texture()
  {
    if (id_)
    {
      glDeleteTextures(1, &id_);
      id_ = 0;
    }
  }

  void Load(const std::string& filename)
  {
    Clear();

    filename_ = filename;
    unsigned char* data = stbi_load_helper(filename.c_str(), &width_, &height_, &num_components_, 0);
    type_ = GL_UNSIGNED_BYTE;

    // Prepare GL resources
    glGenTextures(1, &id_);
    Bind();

    // Set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexStorage2D(GL_TEXTURE_2D, MipmapLevels(), InternalFormat(), width_, height_);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, Format(), type_, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    Unbind();

    stbi_image_free_helper(data);
  }

  void Update(const Image& image)
  {
    if (id_ == 0 ||
      (width_ != image.Width() || height_ != image.Height() || num_components_ != image.NumComponents() || type_ != image.GlType()))
    {
      Clear();

      width_ = image.Width();
      height_ = image.Height();
      num_components_ = image.NumComponents();
      type_ = image.GlType();

      // Prepare GL resources
      glGenTextures(1, &id_);
      Bind();

      // Set the texture wrapping/filtering options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glTexStorage2D(GL_TEXTURE_2D, MipmapLevels(), InternalFormat(), width_, height_);
    }

    Bind();

    if (image.Data<>() != nullptr)
    {
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, Format(), type_, image.Data<>());
    }

    if (MipmapLevels() > 1)
    {
      glGenerateMipmap(GL_TEXTURE_2D);
    }

    Unbind();
  }

  GLenum InternalFormat() const
  {
    switch ((type_ << 4) + num_components_)
    {
    case (GL_UNSIGNED_BYTE  << 4) + 1: return GL_R8;
    case (GL_UNSIGNED_SHORT << 4) + 1: return GL_R16UI;
    case (GL_UNSIGNED_INT   << 4) + 1: return GL_R32UI;
    case (GL_UNSIGNED_BYTE  << 4) + 2: return GL_RG8;
    case (GL_UNSIGNED_SHORT << 4) + 2: return GL_RG16UI;
    case (GL_UNSIGNED_INT   << 4) + 2: return GL_RG32UI;
    case (GL_UNSIGNED_BYTE  << 4) + 3: return GL_RGB8;
    case (GL_UNSIGNED_SHORT << 4) + 3: return GL_RGB16UI;
    case (GL_UNSIGNED_INT   << 4) + 3: return GL_RGB32UI;
    case (GL_UNSIGNED_BYTE  << 4) + 4: return GL_RGBA8;
    case (GL_UNSIGNED_SHORT << 4) + 4: return GL_RGBA16UI;
    case (GL_UNSIGNED_INT   << 4) + 4: return GL_RGBA32UI;
    default: return 0;
    }
  }

  GLenum Format() const
  {
    switch (num_components_)
    {
    case 1: return GL_RED_INTEGER;
    case 2: return GL_RG;
    case 3: return GL_RGB;
    case 4: return GL_RGBA;
    default: return 0;
    }
  }

  int MipmapLevels() const
  {
    switch (type_)
    {
    case GL_UNSIGNED_BYTE: return 3;
    default: return 1;
    }
  }

  void Clear()
  {
    if (id_)
    {
      glDeleteTextures(1, &id_);

      id_ = 0;

      width_ = 0;
      height_ = 0;
      num_components_ = 0;
      filename_.clear();
    }
  }

  void Bind()
  {
    glBindTexture(GL_TEXTURE_2D, id_);
  }

  void Unbind()
  {
    glBindTexture(GL_TEXTURE_2D, 0);
  }

private:
  std::string filename_;

  GLuint id_ = 0;

  int width_ = 0;
  int height_ = 0;
  int num_components_ = 0;
  GLenum type_ = 0;
};
}
}

#endif // PBR_RENDER_GL_GL_TEXTURE_H_
