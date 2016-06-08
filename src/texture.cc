/*
Copyright (C) 2015 Nacho Fuentes Talens
Texture Class
*/

#include "gpumanager.h"
#include "GL\glew.h"

namespace NachosEngine {

  Texture::Texture() {

    internal_texture = new InternalT;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &internal_texture->max_texture_units_);
    internal_texture->textures_in_memory_ = 0;

  }

  Texture::~Texture() {
    delete(internal_texture);
  }

  unsigned int Texture::GenerateEmptyTexture(Format format, unsigned int width, unsigned int height) {
    return internal_texture->GenerateEmptyTexture(format, width, height);
  }

  unsigned int Texture::LoadTexture(const char* texture) {
    return internal_texture->LoadTexture(texture);
  }

  void Texture::DeleteTexture(unsigned int id) {
    internal_texture->DeleteTexture(id);
  }

}//end namespace NachosEngine
