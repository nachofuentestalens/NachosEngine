/*
Copyright (C) 2015 Nacho Fuentes Talens
Frame Buffer class c++ code
*/

#include "NachosEngine\frame_buffer.h"
#include "GL\glew.h"

namespace NachosEngine {

  FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &id_);
	viewport_x = 0;
	viewport_y = 0;
  }

  FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &id_);
  }

  void FrameBuffer::viewport(unsigned int x, unsigned int y) {
	  viewport_x = x;
	  viewport_y = y;
  }

  void FrameBuffer::AttachTexture(Attachment attachment, unsigned int texture_id, int level) {
    switch (attachment) {
    case kAttachment_Color:
      glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_id, level);
      break;
    case kAttachment_Depth:
      glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture_id, level);
      break;
    case kAttachment_DepthStencil:
      glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, texture_id, level);
      break;
    case kAttachment_Stencil:
      glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, texture_id, level);
      break;
    }
  }

  void FrameBuffer::Activate() {
    glBindFramebuffer(GL_FRAMEBUFFER, id_);
	glViewport(0, 0, viewport_x, viewport_y);
	if (viewport_x == 0 && viewport_y == 0) {
		glViewport(0, 0, 800, 600);
	}
  }

  void FrameBuffer::Deactivate() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

}