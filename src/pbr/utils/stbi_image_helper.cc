#include "pbr/utils/stbi_image_helper.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace pbr
{
void stbi_set_flip_vertically_on_load_helper(int flag_true_if_should_flip)
{
  stbi_set_flip_vertically_on_load(flag_true_if_should_flip);
}

unsigned char* stbi_load_helper(const char* filename, int* x, int* y, int* comp, int req_comp)
{
  return stbi_load(filename, x, y, comp, req_comp);
}

unsigned short* stbi_load_16_helper(const char* filename, int* x, int* y, int* comp, int req_comp)
{
  return stbi_load_16(filename, x, y, comp, req_comp);
}

void stbi_image_free_helper(void* retval_from_stbi_load)
{
  stbi_image_free(retval_from_stbi_load);
}
}
