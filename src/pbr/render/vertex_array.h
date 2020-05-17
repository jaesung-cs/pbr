#ifndef PBR_RENDER_VERTEX_ARRAY_H_
#define PBR_RENDER_VERTEX_ARRAY_H_

#include <glad/glad.h>

#include "pbr/render/buffer.h"

namespace pbr
{
class VertexArray
{
public:
  enum class DrawMode : GLenum
  {
    LINES = GL_LINES,
    TRIANGLES = GL_TRIANGLES,
    UNDEFINED = -1
  };

public:
  VertexArray();
  ~VertexArray();

  void AttribPointer(int index, BufferBase& buffer, int size, int stride = 0, int offset = 0);

  void Generate();

  void Bind();
  void Unbind();

  void Update();

  void SetDrawMode(DrawMode mode, int count);
  void Draw();

private:
  GLuint id_ = 0;

  bool generated_ = false;
  bool need_update_ = false;

  DrawMode mode_ = DrawMode::UNDEFINED;
  int count_ = 0;

  struct Attrib
  {
    Attrib(int index, BufferBase& buffer, int size, int stride = 0, int offset = 0)
      : buffer(&buffer), stride(stride), offset(offset)
    {
    }

    int index = 0;
    BufferBase* buffer;
    int size = 1;
    int stride = 0;
    int offset = 0;
  };

  std::vector<Attrib> attribs_;
};
}

#endif // PBR_RENDER_VERTEX_ARRAY_H_
