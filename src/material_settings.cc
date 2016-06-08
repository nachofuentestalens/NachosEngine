/*
Copyright (C) 2015 Nacho Fuentes Talens
Material Class
*/

#include "NachosEngine\material_settings.h"
#include "NachosEngine\scoped_array.h"
#include "GL\glew.h"
#include "gpumanager.h"

namespace NachosEngine {
	MaterialSettings::MaterialSettings() {
    internal_material_settings = new InternalMS;
    internal_material_settings->num_textures_ = 0;
	}

	MaterialSettings::~MaterialSettings(){
    delete(internal_material_settings);
	}

	void MaterialSettings::set_material(Material* mat) {
    internal_material_settings->set_material(mat);
	}

	void MaterialSettings::UniformValue(const char* uniform, const float* data) {
    internal_material_settings->UniformValue(uniform,data);
	}

  void MaterialSettings::UniformValue(const char* uniform, const unsigned int* data) {
    internal_material_settings->UniformValue(uniform, data);
  }

  void MaterialSettings::AddTexture(const unsigned int id) {
    internal_material_settings->AddTexture(id);
  }

  void MaterialSettings::AddSamplerName(const char* name) {
    internal_material_settings->AddSamplerName(name);
  }

  void MaterialSettings::RemoveTexture(const unsigned int id) {
    internal_material_settings->RemoveTexture(id);
  }

  void MaterialSettings::RemoveSamplerName(const char* name) {
    internal_material_settings->RemoveSamplerName(name);
  }

} // end namespace NachosEngine