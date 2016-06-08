/*
Copyright (C) 2015 Nacho Fuentes Talens
Header file for Texture class in NachosEngine
*/

#ifndef __TEXTURE_H__
#define __TEXTURE_H__ 1

#include "referenced.h"

namespace NachosEngine {

class Texture : public Referenced {
  public:
    enum Format {
      kFormat_RGB = 0,
      kFormat_DepthComponent,
      kFormat_DepthSenthil
    };
    static Texture& Instance() {
      static Texture instance;
      return instance;
    }
    struct InternalT;
    InternalT* internal_texture;
    //returns texture id
    unsigned int GenerateEmptyTexture(Format format, unsigned int width, unsigned int height);
    unsigned int LoadTexture(const char* texture);
    void DeleteTexture(unsigned int id);
  protected:


  private:
    Texture();
    Texture(Texture const &t);
    ~Texture();
    Texture operator=(const Texture&);
};

} //end namespace NachosEngine
#endif //__TEXTURE_H__