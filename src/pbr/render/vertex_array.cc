#include "pbr/render/vertex_array.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace pbr
{
VertexArray::VertexArray()
{
}

VertexArray::~VertexArray()
{
  if (generated_)
    glDeleteVertexArrays(1, &id_);
}

void VertexArray::AttribPointer(int index, BufferBase& buffer, int size, int stride, int offset)
{
  attribs_.emplace_back(index, buffer, size, stride, offset);
}

void VertexArray::Generate()
{
  if (!generated_)
  {
    glGenVertexArrays(1, &id_);
    generated_ = true;
  }
}

void VertexArray::Bind()
{
  if (!generated_)
    Generate();

  glBindVertexArray(id_);
}

void VertexArray::Unbind()
{
  glBindVertexArray(0);
}

void VertexArray::Update()
{
  Bind();
  for (auto& attrib : attribs_)
  {
    // TODO: change GL_FLOAT to corresponding data type
    const int data_type_size = attrib.buffer->DataTypeSize();
    attrib.buffer->Bind();
    glVertexAttribPointer(attrib.index, attrib.size, GL_FLOAT, GL_FALSE, attrib.stride * data_type_size, (void*)(attrib.offset * data_type_size));
    glEnableVertexAttribArray(attrib.index);
    attrib.buffer->Unbind();
  }
  Unbind();
}

void VertexArray::SetDrawMode(DrawMode mode, int count)
{
  mode_ = mode;
  count_ = count;
}

void VertexArray::Draw()
{
  Bind();
  glDrawArrays(static_cast<GLenum>(mode_), 0, count_);
  Unbind();
}
}
