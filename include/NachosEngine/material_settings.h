/*
Copyright (C) 2015 Nacho Fuentes Talens
Header file for Material class in NachosEngine
*/

#ifndef __MATERIAL_SETTINGS_H__
#define __MATERIAL_SETTINGS_H__ 1

#include "scoped_array.h"
#include "referenced.h"

#include "material.h"

namespace NachosEngine {
  class MaterialSettings : public Referenced{
  public:
    MaterialSettings();
    ~MaterialSettings();
    void set_material(Material* mat);
    void UniformValue(const char* uniform, const float* data);
    void UniformValue(const char* uniform, const unsigned int* data);
    void AddTexture(const unsigned int id);
    void AddSamplerName(const char* name);
    void RemoveTexture(const unsigned int id);
    void RemoveSamplerName(const char* name);

    struct InternalMS;
    InternalMS* internal_material_settings;

  protected:

  private:
    MaterialSettings(const MaterialSettings&);
    MaterialSettings& operator=(const MaterialSettings&);
    static const unsigned int sampler_name_max_length = 50;

  };
}

#endif // __MATERIAL_SETTINGS_H__