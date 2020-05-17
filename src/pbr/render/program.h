#ifndef PBR_RENDER_PROGRAM_H_
#define PBR_RENDER_PROGRAM_H_

#include <initializer_list>
#include <vector>

#include "pbr/render/shader.h"

namespace pbr
{
class Program
{
public:
  Program();

  Program(const std::string& directory, const std::string& name);

  explicit Program(std::initializer_list<Shader> shaders);

  ~Program();

private:
  std::vector<Shader> shaders_;
};
}

#endif // PBR_RENDER_PROGRAM_H_
