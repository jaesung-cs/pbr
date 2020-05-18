#include "pbr/render/gl/gl_program.h"

#include <iostream>

namespace pbr
{
namespace gl
{
Program::Program()
{
}

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

Program::~Program()
{
  if (generated_)
    glDeleteProgram(id_);
}

void Program::Link()
{
  int success = 0;
  char info_log[512];

  // Shader program
  id_ = glCreateProgram();

  for (auto& shader : shaders_)
  {
    if (!shader.IsCompiled())
      shader.Compile();
  }

  for (const auto& shader : shaders_)
  {
    if (shader.IsGenerated())
      glAttachShader(id_, shader.Id());
  }

  glLinkProgram(id_);
  linked_ = true;

  // Print linking errors if any
  glGetProgramiv(id_, GL_LINK_STATUS, &success);
  if (success)
    generated_ = true;
  else
  {
    glGetProgramInfoLog(id_, 512, NULL, info_log);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;

    glDeleteProgram(id_);
  }

  // Delete shaders
  shaders_.clear();
}

void Program::Use()
{
  if (!linked_)
    Link();

  glUseProgram(id_);
}

void Program::Finished()
{
  glUseProgram(0);
}

void Program::Uniform1i(const std::string& name, int i)
{
  glUniform1i(glGetUniformLocation(id_, name.c_str()), i);
}

void Program::Uniform1f(const std::string& name, float f)
{
  glUniform1f(glGetUniformLocation(id_, name.c_str()), f);
}

void Program::Uniform2f(const std::string& name, float f0, float f1)
{
  glUniform2f(glGetUniformLocation(id_, name.c_str()), f0, f1);
}

void Program::Uniform3f(const std::string& name, const Vector3f& m)
{
  glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, m.data());
}

void Program::Uniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
  glUniform4f(glGetUniformLocation(id_, name.c_str()), f0, f1, f2, f3);
}

void Program::UniformMatrix3f(const std::string & name, const Matrix3f& m)
{
  glUniformMatrix3fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, m.data());
}

void Program::UniformMatrix4f(const std::string& name, const Matrix4f& m)
{
  glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, m.data());
}
}
}
