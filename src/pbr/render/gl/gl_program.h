#ifndef PBR_RENDER_PROGRAM_H_
#define PBR_RENDER_PROGRAM_H_

#include <initializer_list>
#include <vector>

#include <glad/glad.h>

#include "pbr/render/gl/gl_shader.h"
#include "pbr/types.h"

namespace pbr
{
namespace gl
{
class Program
{
public:
  Program();

  Program(const std::string& directory, const std::string& name);

  explicit Program(std::initializer_list<Shader> shaders);

  ~Program();

  void Link();

  void Use();
  void Finished();

  void Uniform1i(const std::string& name, int i);
  void Uniform1f(const std::string& name, float f);
  void Uniform2f(const std::string& name, float f0, float f1);
  void Uniform3f(const std::string& name, const Vector3f& m);
  void Uniform4f(const std::string& name, float f0, float f1, float f2, float f3);
  void UniformMatrix4f(const std::string& name, const Matrix4f& m);

private:
  GLuint id_ = 0;

  bool linked_ = false;
  bool generated_ = false;
  std::vector<Shader> shaders_;
};
}
}

#endif // PBR_RENDER_PROGRAM_H_
