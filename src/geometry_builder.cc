/*
Copyright (C) 2015 Nacho Fuentes Talens
Implementation of GeometryBuilder class
*/

#include "NachosEngine\geometry_builder.h"

namespace NachosEngine {
  GeometryBuilder::GeometryBuilder() {

    num_vertices_ = 0;
    total_attribute_floats_ = 0;
    total_size_ = 0;

    for (unsigned int i = 0; i < kAttributeName_Max; ++i){
      attrib_mask_[i] = kAttributeType_None;
      geo_attrib_info_[i].attrib_name = kAttributeName_None;
      geo_attrib_info_[i].attrib_type = kAttributeType_None;
      geo_attrib_info_[i].offset = 0;
    }

  }

  GeometryBuilder::~GeometryBuilder() {

  }

  void GeometryBuilder::Begin(unsigned int num_vertex) {
    num_vertices_ = num_vertex;
    vertices_.alloc(num_vertices_);
  }
  void GeometryBuilder::End(Geometry* geo, const unsigned int *elements, const unsigned int elements_quantity) {

    Order();

    geo->set_geo_attrib_info(geo_attrib_info_);

    geo->set_mask(attrib_mask_);

    geo->set_elements_quantity(elements_quantity);

    geo->set_data(ordered_vertices_values_.get(), total_size_);

    geo->set_elements(elements, sizeof(unsigned int)*elements_quantity);

  }
  void GeometryBuilder::Add(AttribName attrib_name, AttribType attrib_type, const float *values) {

    attrib_mask_[attrib_name] = attrib_type;

    geo_attrib_info_[attrib_name].attrib_name = attrib_name;

    geo_attrib_info_[attrib_name].attrib_type = attrib_type;

    total_attribute_floats_ += attrib_type;

    unsigned int current_value_position = 0;

    for (unsigned int i = 0; i < num_vertices_; ++i) {
      switch (attrib_type) {
      case kAttributeType_Float:
        vertices_[i].Attribute_Information[attrib_name].values[0] = values[current_value_position];
        current_value_position++;
        break;
      case kAttributeType_Float2:
        vertices_[i].Attribute_Information[attrib_name].values[0] = values[current_value_position];
        vertices_[i].Attribute_Information[attrib_name].values[1] = values[current_value_position + 1];
        current_value_position += 2;
        break;
      case kAttributeType_Float3:
        vertices_[i].Attribute_Information[attrib_name].values[0] = values[current_value_position];
        vertices_[i].Attribute_Information[attrib_name].values[1] = values[current_value_position + 1];
        vertices_[i].Attribute_Information[attrib_name].values[2] = values[current_value_position + 2];
        current_value_position += 3;
        break;
      case kAttributeType_Float4:
        vertices_[i].Attribute_Information[attrib_name].values[0] = values[current_value_position];
        vertices_[i].Attribute_Information[attrib_name].values[1] = values[current_value_position + 1];
        vertices_[i].Attribute_Information[attrib_name].values[2] = values[current_value_position + 2];
        vertices_[i].Attribute_Information[attrib_name].values[3] = values[current_value_position + 3];
        current_value_position += 4;
        break;
      }
    }
  }

  void GeometryBuilder::Order() {

    ordered_vertices_values_.alloc(num_vertices_*total_attribute_floats_);

    unsigned int current_value_position = 0;

    for (unsigned int i = 1; i < kAttributeName_Max; ++i) {
      if (attrib_mask_[i] != kAttributeType_None) {
        for (unsigned int j = 0; j < num_vertices_; ++j) {
          switch (geo_attrib_info_[i].attrib_type) {
          case kAttributeType_Float:
            ordered_vertices_values_[current_value_position] = vertices_[j].Attribute_Information[i].values[0];
            current_value_position++;
            break;
          case kAttributeType_Float2:
            ordered_vertices_values_[current_value_position] = vertices_[j].Attribute_Information[i].values[0];
            ordered_vertices_values_[current_value_position + 1] = vertices_[j].Attribute_Information[i].values[1];
            current_value_position += 2;
            break;
          case kAttributeType_Float3:
            ordered_vertices_values_[current_value_position] = vertices_[j].Attribute_Information[i].values[0];
            ordered_vertices_values_[current_value_position + 1] = vertices_[j].Attribute_Information[i].values[1];
            ordered_vertices_values_[current_value_position + 2] = vertices_[j].Attribute_Information[i].values[2];
            current_value_position += 3;
            break;
          case kAttributeType_Float4:
            ordered_vertices_values_[current_value_position] = vertices_[j].Attribute_Information[i].values[0];
            ordered_vertices_values_[current_value_position + 1] = vertices_[j].Attribute_Information[i].values[1];
            ordered_vertices_values_[current_value_position + 2] = vertices_[j].Attribute_Information[i].values[2];
            ordered_vertices_values_[current_value_position + 3] = vertices_[j].Attribute_Information[i].values[3];
            current_value_position += 4;
            break;
          }
        }
        geo_attrib_info_[i].offset = total_size_;
        total_size_ += sizeof(float)*attrib_mask_[i]*num_vertices_;
      }
    }
  }
} //end namespace NachosEngine