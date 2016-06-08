/*
Copyright (C) 2015 Nacho Fuentes Talens
Drawable Class
*/

#include "NachosEngine\drawable.h"

#define GLM_FORCE_RADIANS
#include "glm\glm.hpp"
#include "glm\ext.hpp"

namespace NachosEngine {
  
  Drawable::Drawable(){
	  postprocess_ = 0;
  }

  Drawable::~Drawable(){
  
  }

  Geometry* Drawable::geometry() {
    return geometry_.get();
  }
  Material* Drawable::material() {
    return material_.get();
  }
  MaterialSettings* Drawable::materialsettings() {
    return material_settings_.get();
  }


  void Drawable::set_geometry(Geometry* geo) {
    geometry_ = geo;
  }
  void Drawable::set_material(Material* mat) {
    material_ = mat;
  }
  void Drawable::set_materialsettings(MaterialSettings* ms) {
    material_settings_ = ms;
  }

} //NachosEngine