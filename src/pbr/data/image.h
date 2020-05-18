#ifndef PBR_DATA_IMAGE_H_
#define PBR_DATA_IMAGE_H_

#include <string>
#include <future>

#include <glad/glad.h>

#include "pbr/types.h"
#include "pbr/utils/stbi_image_helper.h"

namespace pbr
{
class Image
{
public:
  enum class Type
  {
    UNSIGNED_BYTE,
    UNSIGNED_SHORT,
    UNSIGNED_INT,
    UNDEFINED,
  };

public:
  Image()
    : Image(1, 1, 1, { (unsigned char)0 })
  {
  }

  explicit Image(const std::string& filename)
  {
    Load(filename);
  }

  template <typename T>
  Image(int width, int height, int num_components, T default_value)
  {
    width_ = width;
    height_ = height;
    num_components_ = num_components;

    type_ = DataType<T>();
    auto arr = new T[static_cast<size_t>(width) * height * num_components];

    for (size_t i = 0; i < width * height * num_components; i++)
      arr[i] = default_value;

    data_ = arr;
  }

  template <typename T>
  Image(int width, int height, int num_components, std::initializer_list<T> data)
  {
    width_ = width;
    height_ = height;
    num_components_ = num_components;

    type_ = DataType<T>();
    auto arr = new T[static_cast<size_t>(width) * height * num_components];

    size_t i = 0;
    for (auto c : data)
      arr[i++] = c;

    data_ = arr;
  }

  ~Image()
  {
    Clear();
  }

  // Copy constructors
  template <typename T>
  void* CreateCopyData(const Image& rhs)
  {
    const int buffer_size = rhs.width_ * rhs.height_ * rhs.num_components_;

    T* result = new T[buffer_size];
    auto arr = reinterpret_cast<T*>(rhs.data_);

    for (int i = 0; i < buffer_size; i++)
      result[i] = arr[i];

    return result;
  }

  template <typename T>
  void CopyData(const Image& rhs)
  {
    const int buffer_size = rhs.width_ * rhs.height_ * rhs.num_components_;

    auto data = reinterpret_cast<T*>(data_);
    auto arr = reinterpret_cast<T*>(rhs.data_);

    for (int i = 0; i < buffer_size; i++)
      data[i] = arr[i];
  }

  void Copy(const Image& rhs)
  {
    if (width_ == rhs.width_ && height_ == rhs.height_ && num_components_ == rhs.num_components_ && type_ == rhs.type_ &&
      data_ && rhs.data_)
    {
      // Have buffers of same size, so copy data without new/delete
      switch (type_)
      {
      case Type::UNSIGNED_BYTE:
        CopyData<unsigned char>(rhs);
        break;
      case Type::UNSIGNED_SHORT:
        CopyData<unsigned short>(rhs);
        break;
      case Type::UNSIGNED_INT:
        CopyData<unsigned int>(rhs);
        break;
      }
    }

    else
    {
      width_ = rhs.width_;
      height_ = rhs.height_;
      num_components_ = rhs.num_components_;
      type_ = rhs.type_;

      if (data_)
      {
        delete data_;
        data_ = nullptr;
      }

      if (rhs.data_ && width_ * height_ * num_components_ > 0)
      {
        switch (type_)
        {
        case Type::UNSIGNED_BYTE:
          data_ = CreateCopyData<unsigned char>(rhs);
          break;
        case Type::UNSIGNED_SHORT:
          data_ = CreateCopyData<unsigned short>(rhs);
          break;
        case Type::UNSIGNED_INT:
          data_ = CreateCopyData<unsigned int>(rhs);
          break;
        }
      }
      else
        data_ = nullptr;
    }
  }

  Image(const Image& rhs)
  {
    Copy(rhs);
  }

  Image& operator = (const Image& rhs)
  {
    Copy(rhs);
    return *this;
  }

  // Move constructors
  void Move(Image&& rhs) noexcept
  {
    Clear();

    filename_ = std::move(rhs.filename_);

    width_ = rhs.width_;
    height_ = rhs.height_;
    num_components_ = rhs.num_components_;
    type_ = rhs.type_;
    data_ = rhs.data_;

    rhs.width_ = 0;
    rhs.height_ = 0;
    rhs.num_components_ = 0;
    rhs.data_ = 0;
    rhs.type_ = Type::UNDEFINED;
  }

  Image(Image&& rhs) noexcept
  {
    Move(std::move(rhs));
  }

  Image& operator = (Image&& rhs) noexcept
  {
    Move(std::move(rhs));
    return *this;
  }

  void Clear()
  {
    if (data_)
      delete data_;

    filename_.clear();

    width_ = 0;
    height_ = 0;
    num_components_ = 0;
  }

  auto Width() const noexcept { return width_; }
  auto Height() const noexcept { return height_; }
  auto NumComponents() const noexcept { return num_components_; }
  const auto& Filename() const noexcept { return filename_; }

  template <typename T = void>
  auto Data() const noexcept { return reinterpret_cast<T*>(data_); }

  auto GlType() const noexcept
  {
    switch (type_)
    {
    case Type::UNSIGNED_BYTE:  return GL_UNSIGNED_BYTE;
    case Type::UNSIGNED_SHORT: return GL_UNSIGNED_SHORT;
    case Type::UNSIGNED_INT:   return GL_UNSIGNED_INT;
    default: return 0;
    }
  }

  template <typename T = unsigned char>
  void Load(const std::string& filename)
  {
  }

  template <>
  void Load<unsigned char>(const std::string& filename)
  {
    // Delete any previous texture
    if (data_)
      delete data_;

    filename_ = filename;
    type_ = Type::UNSIGNED_BYTE;
    unsigned char* data = stbi_load_helper(filename.c_str(), &width_, &height_, &num_components_, 0);

    data_ = data;
  }

  template <>
  void Load<unsigned short>(const std::string& filename)
  {
    // Delete any previous texture
    if (data_)
      delete data_;

    filename_ = filename;
    type_ = Type::UNSIGNED_SHORT;
    unsigned short* data = stbi_load_16_helper(filename.c_str(), &width_, &height_, &num_components_, 0);

    data_ = data;
  }

private:
  template <typename T> constexpr Type DataType() const noexcept { return Type::UNDEFINED; }
  template <> constexpr Type DataType<unsigned char>() const noexcept { return Type::UNSIGNED_BYTE; }
  template <> constexpr Type DataType<unsigned short>() const noexcept { return Type::UNSIGNED_SHORT; }
  template <> constexpr Type DataType<unsigned int>() const noexcept { return Type::UNSIGNED_INT; }

  std::string filename_;

  int width_ = 0;
  int height_ = 0;
  int num_components_ = 0;
  Type type_ = Type::UNDEFINED;
  void* data_ = 0;
};
}

#endif // PBR_DATA_IMAGE_H_
