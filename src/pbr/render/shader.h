#ifndef PBR_RENDER_SHADER_H_
#define PBR_RENDER_SHADER_H_

#include <string>

namespace pbr
{
class Shader
{
public:
  enum class Type
  {
    VERTEX,
    GEOMETRY,
    FRAGMENT,
    UNDEFINED,
  };

public:
  Shader();

  explicit Shader(const std::string& filename);

  ~Shader();

  void LoadFile(const std::string& filename);
  void LoadFile(Type type, const std::string& filename);

private:
  Type type_ = Type::UNDEFINED;
  std::string filename_;
  std::string code_;
};
}

#endif // PBR_RENDER_SHADER_H_
