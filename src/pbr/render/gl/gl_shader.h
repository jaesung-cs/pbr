#ifndef PBR_RENDER_SHADER_H_
#define PBR_RENDER_SHADER_H_

#include <string>

#include <glad/glad.h>

namespace pbr
{
namespace gl
{
class Shader
{
public:
  enum class Type : GLenum
  {
    VERTEX = GL_VERTEX_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    UNDEFINED,
  };

public:
  Shader();

  explicit Shader(const std::string& filename);

  ~Shader();

  // Copy
  void Copy(const Shader& rhs)
  {
    type_ = rhs.type_;
    filename_ = rhs.filename_;
    code_loaded_ = rhs.code_loaded_;
    code_ = rhs.code_;

    if (generated_)
      Compile();
  }

  Shader(const Shader& rhs)
  {
    Copy(rhs);
  }

  Shader& operator = (const Shader& rhs)
  {
    Copy(rhs);
    return *this;
  }

  // Move
  Shader(Shader&& rhs) = default;

  Shader& operator = (Shader&& rhs) = default;

  bool IsCompiled() const
  {
    return compiled_;
  }

  bool IsGenerated() const
  {
    return generated_;
  }

  auto Id() const
  {
    return id_;
  }

  void LoadFile(const std::string& filename);
  void LoadFile(Type type, const std::string& filename);
  void Compile();

private:
  GLuint id_ = 0;

  bool code_loaded_ = false;
  bool compiled_ = false;
  bool generated_ = false;
  Type type_ = Type::UNDEFINED;
  std::string filename_;
  std::string code_;
};
}
}

#endif // PBR_RENDER_SHADER_H_
