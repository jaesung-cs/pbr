#include <stdexcept>
#include <iostream>

#include "pbr/engine.h"

int main()
{
  pbr::Engine engine;

  try
  {
    engine.Run();
  }
  catch (std::exception e)
  {
    std::cout << e.what() << std::endl;
  }

  return 0;
}

// glfw
// assimp
// stbi
