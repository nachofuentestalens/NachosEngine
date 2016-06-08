/*
Copyright (C) 2015 Nacho Fuentes Talens
Header file for Buffer class in NachosEngine
*/

#ifndef __BUFFER_H__
#define __BUFFER_H__ 1

#include "scoped_ptr.h"

namespace NachosEngine {
  class Buffer {
  public:
    enum Target {
      kAttribute = 1,
      kElement = 2
    };
    enum Attribute {
      A_POSITION = 1,
      A_COLOR = 2,
      A_NORMAL = 3,
      A_UV = 4
    };

    struct InternalB;
    scoped_ptr<InternalB> internal_buffer;

    Buffer();
    ~Buffer();
    void init(unsigned int size);
    void init(unsigned int size, Target t);
    void Upload(const void* data,
      const Target t,
      const unsigned int size,
      const unsigned int offset = 0);
    
  protected:

  private:
    Buffer& operator=(const Buffer&){}
    Buffer(const Buffer&){}
  };
} //End namepsace NachosEngine


#endif //__BUFFER_H__