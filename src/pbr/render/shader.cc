#include "pbr/render/shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace pbr
{
Shader::Shader()
{
}

Shader::Shader(const std::string& filename)
{
  filename_ = filename;
}

Shader::~Shader() = default;

void Shader::LoadFile(const std::string& filename)
{
  auto length = filename.length();
  if (length >= 6)
  {
    auto ext = filename.substr(length - 5);

    if (ext == ".vert")
      LoadFile(Type::VERTEX, filename);

    else if (ext == ".geom")
      LoadFile(Type::GEOMETRY, filename);

    else if (ext == ".frag")
      LoadFile(Type::FRAGMENT, filename);
  }
}

void Shader::LoadFile(Type type, const std::string& filename)
{
  type_ = type;
  filename_ = filename;

  std::ifstream shader_file;

  // Ensure ifstream objects can throw exceptions:
  shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    // Open files
    shader_file.open(filename);

    // Read file's buffer contents into streams
    std::stringstream shader_stream;
    shader_stream << shader_file.rdbuf();

    // Close file handlers
    shader_file.close();

    // Convert stream into string
    code_ = shader_stream.str();
  }
  catch (std::ifstream::failure e)
  {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }
}
}
