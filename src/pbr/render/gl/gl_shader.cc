#include "pbr/render/gl/gl_shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace pbr
{
namespace gl
{
Shader::Shader()
{
}

Shader::Shader(const std::string& filename)
{
  filename_ = filename;
}

Shader::~Shader()
{
  if (generated_)
    glDeleteShader(id_);
}

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
    code_loaded_ = true;
    compiled_ = false;

    if (IsGenerated())
    {
      glDeleteShader(id_);
      id_ = 0;
      generated_ = false;
    }
  }
  catch (std::ifstream::failure e)
  {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }
}

void Shader::Compile()
{
  if (!code_loaded_)
    LoadFile(filename_);

  if (code_loaded_)
  {
    const char* code_ptr = code_.c_str();

    // Compile shaders
    int success = 0;
    char info_log[512];

    // Vertex Shader
    id_ = glCreateShader(static_cast<GLenum>(type_));
    glShaderSource(id_, 1, &code_ptr, NULL);
    glCompileShader(id_);
    compiled_ = true;

    // Check compile errors
    glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
    if (success)
      generated_ = true;
    else
    {
      // Print compile errors if any
      glGetShaderInfoLog(id_, 512, NULL, info_log);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;

      glDeleteShader(id_);
      id_ = 0;
    }
  }
}
}
}
