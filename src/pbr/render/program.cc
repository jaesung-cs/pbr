#include "pbr/render/program.h"

#include <iostream>

namespace pbr
{
Program::Program() = default;

Program::Program(const std::string& directory, const std::string& name)
{
  shaders_.emplace_back(directory + "\\" + name + ".vert");
  shaders_.emplace_back(directory + "\\" + name + ".geom");
  shaders_.emplace_back(directory + "\\" + name + ".frag");
}

Program::Program(std::initializer_list<Shader> shaders)
  : shaders_(std::move(shaders))
{
}

Program::~Program() = default;
}
