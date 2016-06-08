/*
Copyright (C) 2015 Nacho Fuentes Talens
Header file for GeometryBuilder
*/

#ifndef __GEOMETRY_BUILDER_H__
#define __GEOMETRY_BUILDER_H__ 1

#include "ref_ptr.h"
#include "scoped_array.h"

#include "geometry.h"
#include "typedefs.h"

namespace NachosEngine {

  class GeometryBuilder {
  public:

    struct AttributeValues {
      float values[4];
    };
    struct VerticesInformation {
      AttributeValues Attribute_Information[kAttributeName_Max];
    };

    GeometryBuilder();
    ~GeometryBuilder();

    void Begin(unsigned int num_vertex);
    void End(Geometry* geo, const unsigned int *elements, const unsigned int elements_size);
    void Add(AttribName attrib_name, AttribType attrib_type, const float  *values);
    void Order();

  protected:


  private:
    unsigned int num_vertices_;
    unsigned int total_attribute_floats_;
    //can remove total_size_ and use sizeof(float)*ordered_vertices_values_.size()
    unsigned int total_size_;
    AttribMask attrib_mask_;
    Geometry::GeometryAttributeInformation geo_attrib_info_[kAttributeName_Max];
    scoped_array<VerticesInformation> vertices_;

    scoped_array<float> ordered_vertices_values_;

    
    
    
  };

} //end namespace NachosEngine

#endif //__GEOMETRY_BUILDER_H__