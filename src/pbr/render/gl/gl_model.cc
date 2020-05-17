#include "pbr/render/gl/gl_model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace pbr
{
namespace gl
{
Model::Model()
{
}

Model::~Model()
{
  if (generated_)
    Clear();
}

void Model::Load(const std::string& filename, bool recenter)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);
  if (scene == NULL)
    return;

  // If already loaded, clear the previous
  if (vao_)
    Clear();

  filename_ = filename;

  // Prepare GL resources
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  // Move assimp data to GL memory
  aiNode* scene_node = scene->mRootNode;

  // Assume only one mesh is contained in the file
  aiMesh* mesh = scene->mMeshes[0];

  // Vertex attributes
  num_vertices_ = mesh->mNumVertices;

  int num_attribute_elements = num_vertices_ * 3;
  if (mesh->mNormals != NULL)
    num_attribute_elements += num_vertices_ * 3;
  if (mesh->mTextureCoords[0] != NULL)
    num_attribute_elements += num_vertices_ * 2;

  // xyz range
  float xmin = mesh->mVertices[0].x;
  float xmax = xmin;
  float ymin = mesh->mVertices[0].y;
  float ymax = ymin;
  float zmin = mesh->mVertices[0].z;
  float zmax = zmin;
  for (int i = 1; i < mesh->mNumVertices; i++)
  {
    if (xmin > mesh->mVertices[i].x)
      xmin = mesh->mVertices[i].x;
    if (xmax < mesh->mVertices[i].x)
      xmax = mesh->mVertices[i].x;
    if (ymin > mesh->mVertices[i].y)
      ymin = mesh->mVertices[i].y;
    if (ymax < mesh->mVertices[i].y)
      ymax = mesh->mVertices[i].y;
    if (zmin > mesh->mVertices[i].z)
      zmin = mesh->mVertices[i].z;
    if (zmax < mesh->mVertices[i].z)
      zmax = mesh->mVertices[i].z;
  }

  float xcenter = (xmin + xmax) / 2.f;
  float ycenter = (ymin + ymax) / 2.f;
  float zcenter = (zmin + zmax) / 2.f;

  // Allocate vertex attribute buffer
  glGenBuffers(1, &vbo_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferStorage(GL_ARRAY_BUFFER, sizeof(float) * num_attribute_elements, NULL, GL_MAP_WRITE_BIT);

  float* ptr = (float*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(float) * num_attribute_elements, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
  int offset = 0;
  for (int i = 0; i < mesh->mNumVertices; i++)
  {
    if (recenter)
    {
      *(ptr++) = mesh->mVertices[i].x - xcenter;
      *(ptr++) = mesh->mVertices[i].y - ycenter;
      *(ptr++) = mesh->mVertices[i].z - zcenter;
    }
    else
    {
      *(ptr++) = mesh->mVertices[i].x;
      *(ptr++) = mesh->mVertices[i].y;
      *(ptr++) = mesh->mVertices[i].z;
    }
  }
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);
  offset += num_vertices_ * 3;

  if (mesh->mNormals != NULL)
  {
    for (int i = 0; i < mesh->mNumVertices; i++)
    {
      *(ptr++) = mesh->mNormals[i].x;
      *(ptr++) = mesh->mNormals[i].y;
      *(ptr++) = mesh->mNormals[i].z;
    }
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * offset));
    glEnableVertexAttribArray(1);
    offset += num_vertices_ * 3;
  }

  if (mesh->mTextureCoords[0] != NULL)
  {
    for (int i = 0; i < mesh->mNumVertices; i++)
    {
      *(ptr++) = mesh->mTextureCoords[0][i].x;
      *(ptr++) = mesh->mTextureCoords[0][i].y;
    }
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * offset));
    glEnableVertexAttribArray(2);
    offset += num_vertices_ * 2;
  }

  glUnmapBuffer(GL_ARRAY_BUFFER);

  // Triangle vertex indices
  num_elements_ = mesh->mNumFaces * 3;

  glGenBuffers(1, &ibo_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
  glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_elements_, NULL, GL_MAP_WRITE_BIT);
  unsigned int* iptr = (unsigned int*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * num_elements_, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
  for (int i = 0; i < mesh->mNumFaces; i++)
  {
    const aiFace& face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; j++)
      *(iptr++) = face.mIndices[j];
  }
  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

  // Texture
  has_diffuse_texture_ = false;
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  int num_diffuse_textures = material->GetTextureCount(aiTextureType_DIFFUSE);
  for (int i = 0; i < num_diffuse_textures; i++)
  {
    aiString str;
    material->GetTexture(aiTextureType_DIFFUSE, i, &str);
    diffuse_texture_filename_ = str.C_Str();

    // Make absolute path
    diffuse_texture_filename_ = filename.substr(0, filename.find_last_of("\\/") + 1) + diffuse_texture_filename_;
    has_diffuse_texture_ = true;
  }

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::Draw()
{
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, num_elements_, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Model::Clear()
{
  filename_ = "";

  if (generated_)
  {
    generated_ = false;
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ibo_);
    vao_ = 0;
    vbo_ = 0;
    ibo_ = 0;
  }

  num_vertices_ = 0;
  num_elements_ = 0;

  has_diffuse_texture_ = false;
  diffuse_texture_filename_ = "";
}
}
}
