/*
Copyright (C) 2015 Nacho Fuentes Talens
Header file for Frame Buffer class in NachosEngine
*/

#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__ 1

#include "referenced.h"

namespace NachosEngine {

  class FrameBuffer{
  public:
    enum Attachment {
      kAttachment_Color = 0,
      kAttachment_Depth,
      kAttachment_Stencil,
      kAttachment_DepthStencil
    };


    FrameBuffer();
    ~FrameBuffer();

    void AttachTexture(Attachment attachment, unsigned int texture_id, int level = 0);
	void viewport(unsigned int x, unsigned int y);
    void Activate();
    void Deactivate();
    unsigned int id() {return id_;}

  protected:

  private:
    unsigned int id_;
	unsigned int viewport_x;
	unsigned int viewport_y;

  };

}
#endif //__FRAME_BUFFER_H__