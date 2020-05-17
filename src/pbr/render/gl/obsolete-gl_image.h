#ifndef PBR_RENDER_GL_GL_IMAGE_H_
#define PBR_RENDER_GL_GL_IMAGE_H_

#include <string>
#include <vector>

#include <glad/glad.h>

#include "pbr/types.h"
#include "pbr/data/Image.h"

namespace pbr
{
namespace gl
{
class Image
{
public:
  Image() = delete;

  Image(int width, int height, int num_components);

  ~Image();

  void Update(unsigned char* data);
  void Update(const pbr::Image& image);

  void Draw();

private:
  void Clear();
  void Generate();

  bool generated_ = false;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
  GLuint tex_ = 0;

  int width_ = 0;
  int height_ = 0;
  int num_components_ = 0;
};
}
}

#endif // PBR_RENDER_GL_GL_IMAGE_H_
