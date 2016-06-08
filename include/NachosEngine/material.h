/*
Copyright (C) 2015 Nacho Fuentes Talens
Header file for Material class in NachosEngine
*/

#ifndef __MATERIAL_H__
#define __MATERIAL_H__ 1

#include "ref_ptr.h"
#include "scoped_array.h"
#include "referenced.h"
#include "scoped_ptr.h"

namespace NachosEngine {
  class Material : public Referenced{
   
  public:
    enum DataType {
      FLOAT_1 = 1,
      FLOAT_2 = 2,
      FLOAT_3 = 3,
      FLOAT_4 = 4,
      MAT4x4 = 5,
      SAMPLER_2D,
    };
    Material();
    ~Material();
    unsigned int LoadShader(const char* vs, const char* fs);
    void ReleaseMaterial();
    struct InternalM;
    scoped_ptr<InternalM> internal_material;
    
  protected:
    

  private:
    
    Material(const Material&);
    Material& operator=(const Material&);

  };

} //end namespace NachosEngine
#endif //__MATERIAL_H__