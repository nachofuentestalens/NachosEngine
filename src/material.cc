/*
Copyright (C) 2015 Nacho Fuentes Talens
Material Class
*/

#include "NachosEngine\material.h"

#include "NachosEngine\scoped_array.h"
#include "GL\glew.h"
#include "gpumanager.h"

namespace NachosEngine {
  Material::Material() {

    internal_material.alloc();
    internal_material->attribute_position_ = 0;
    internal_material->attribute_color_ = 0;

    for (int i = 0; i < kAttributeName_Max; ++i) {
      internal_material->mask_[i] = kAttributeType_None;
    }

  }

  Material::~Material(){
    internal_material->ReleaseMaterial();
  }

  unsigned int Material::LoadShader(const char *vs, const char *fs) {
    return internal_material->LoadShader(vs, fs);
  }

  void Material::ReleaseMaterial() {
    internal_material->ReleaseMaterial();
  }

} //end namespace NachosEngine