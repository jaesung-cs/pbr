#ifndef PBR_UTILS_STBI_IMAGE_HELPER_H_
#define PBR_UTILS_STBI_IMAGE_HELPER_H_

namespace pbr
{
void stbi_set_flip_vertically_on_load_helper(int flag_true_if_should_flip);
unsigned char* stbi_load_helper(const char* filename, int* x, int* y, int* comp, int req_comp);
unsigned short* stbi_load_16_helper(const char* filename, int* x, int* y, int* comp, int req_comp);
void stbi_image_free_helper(void* data);
}

#endif // PBR_UTILS_STBI_IMAGE_HELPER_H_