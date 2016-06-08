/*
Copyright (C) 2015 Nacho Fuentes Talens
Header file for Geometry class in NachosEngine
*/

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__ 1

#include "referenced.h"
#include "ref_ptr.h"
#include "scoped_array.h"

#include "buffer.h"
#include "typedefs.h"


namespace NachosEngine {
  


  class Geometry : public Referenced{
  public:
    struct GeometryAttributeInformation {
      AttribType attrib_type;
      AttribName attrib_name;
      unsigned int offset;
    };
    Geometry();
    
    void set_data(const void* data,
      unsigned int size,
      unsigned int offset = 0);
    void set_elements(const void* data,
      unsigned int size,
      unsigned int offset = 0);
    void set_mask(AttribMask mask);
    void set_geo_attrib_info(GeometryAttributeInformation geo_attrib_info[]);
    AttribType geo_attrib_type(unsigned int idx);
    unsigned int geo_attrib_offset(unsigned int idx);
    void set_elements_quantity(unsigned int quantity);
    unsigned int elements_quantity();
    void LoadGeometryFromFile(const char* file_to_load);


    AttribType mask(int idx){return mask_[idx];}

    Buffer* data();
    Buffer* elements();
  protected:

  private:
    ~Geometry();
    Geometry(const Geometry&);
    Geometry operator=(const Geometry&);
    Buffer data_;
    Buffer elements_;
    AttribMask mask_;
    scoped_array<float> attribute_offsets_;
    GeometryAttributeInformation geo_attrib_info_[kAttributeName_Max];
    unsigned int elements_quantity_;
  };

} //end namespace NachosEngine

#endif //__GEOMETRY_H__