/*
Copyright (C) 2015 Nacho Fuentes Talens
Header file for Drawable class in NachosEngine
*/

#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__ 1

#include "node.h"

namespace NachosEngine {

class Drawable : public Node {
  public:

    Drawable();
    ~Drawable();
    
    Geometry* geometry();
    Material* material();
    MaterialSettings* materialsettings();

    void set_geometry(Geometry* geo);
    void set_material(Material* mat);
    void set_materialsettings(MaterialSettings* ms);

	//first bit for glow
	int postprocess_;
  protected:

  private:

    ref_ptr<Geometry> geometry_;
    ref_ptr<Material> material_;
    ref_ptr<MaterialSettings> material_settings_;
	

};

} //namespace NachosEngine

#endif //__DRAWABLE_H__
