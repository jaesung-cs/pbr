#ifndef PBR_RENDER_BUFFER_H_
#define PBR_RENDER_BUFFER_H_

#include <string>
#include <vector>
#include <initializer_list>

#include <glad/glad.h>

namespace pbr
{
class Buffer
{
public:
  enum class Type : GLenum
  {
    ARRAY = GL_ARRAY_BUFFER,
    ELEMENT = GL_ELEMENT_ARRAY_BUFFER,
    UNDEFINED = -1
  };

  enum class Usage : GLenum
  {
    STATIC_DRAW = GL_STATIC_DRAW,
    DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
    UNDEFINED = -1
  };

public:
  Buffer() = default;

  virtual ~Buffer()
  {
    if (generated_)
      glDeleteBuffers(1, &id_);
  }


  // Copy
  void Copy(const Buffer& rhs)
  {
    type_ = rhs.type_;
    usage_ = rhs.usage_;

    if (rhs.generated_)
    {
      Generate();

      // TODO: copy buffer data
    }
    else
    {
      generated_ = false;
    }
  }

  Buffer(const Buffer& rhs)
  {
    Copy(rhs);
  }

  Buffer& operator = (const Buffer& rhs)
  {
    Copy(rhs);
    return *this;
  }

  // Move
  Buffer(Buffer&& rhs) = default;

  Buffer& operator = (Buffer&& rhs) = default;


  // Virtual functions
  virtual void Update()
  {
  }

  virtual int DataTypeSize() const
  {
    return 1;
  }


  bool IsGenerated() const
  {
    return generated_;
  }

  void Generate(Type type, Usage usage)
  {
    type_ = type;
    usage_ = usage;

    Generate();
  }

  void Generate()
  {
    glGenBuffers(1, &id_);
    generated_ = true;
  }


  GLuint Id() const
  {
    return id_;
  }

  void Bind()
  {
    glBindBuffer(static_cast<GLenum>(type_), id_);
  }

  void Unbind()
  {
    glBindBuffer(static_cast<GLenum>(type_), 0);
  }


private:
  GLuint id_ = 0;

  bool generated_ = false;
  Type type_ = Type::UNDEFINED;
  Usage usage_ = Usage::UNDEFINED;
};

template <typename T>
class BufferArray : public Buffer
{
private:
  // Reference
  class Ref
  {
  public:
    Ref() = delete;

    Ref(Buffer& buffer, int idx)
      : buffer_(buffer), idx_(idx)
    {
    }

    Ref& operator = (T value)
    {
      buffer_.Modify(idx_, value);
      return *this;
    }

  private:
    Buffer& buffer_;
    int idx_ = 0;
  };

public:
  BufferArray() = delete;

  explicit BufferArray(int n)
    : Buffer(),
    data_(n, static_cast<T>(0))
  {
    need_update_ = true;
  }

  BufferArray(int n, T value)
    : Buffer(),
    data_(n, value)
  {
    need_update_ = true;
  }

  BufferArray(std::initializer_list<T> values)
    : Buffer(),
    data_(std::move(values))
  {
    need_update_ = true;
  }

  ~BufferArray() override
  {
  }


  // Copy
  void Copy(const BufferArray& rhs)
  {
    need_update_ = rhs.need_update_;
    data_ = rhs.data_;
  }

  BufferArray(const Buffer& rhs)
    : BufferBase(rhs)
  {
    Copy(rhs);
  }

  Buffer& operator = (const Buffer& rhs)
  {
    BufferArray::Copy(rhs)
    Copy(rhs);
    return *this;
  }

  // Move
  Buffer(Buffer&& rhs) = default;

  Buffer& operator = (Buffer&& rhs) = default;


  // Virtual functions
  void Update() override
  {
    if (need_update_)
    {
      if (type_ == Type::UNDEFINED ||
        usage_ == Usage::UNDEFINED)
        return;

      if (!generated_)
        Generate();

      Bind();

      glBufferSubData(static_cast<GLenum>(type_), 0, sizeof(T) * data_.size(), data_.data());

      Unbind();
      need_update_ = false;
    }
  }

  int DataTypeSize() const override
  {
    return sizeof(T);
  }


  // Index operator
  Ref operator [] (int idx)
  {
    return Ref(*this, idx);
  }


  void Modify(int idx, T value)
  {
    data_[idx] = value;
    need_update_ = true;
  }

private:
  bool need_update_ = false;
  std::vector<T> data_;
};
}

#endif // PBR_RENDER_BUFFER_H_
