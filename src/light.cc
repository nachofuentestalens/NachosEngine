/*
Copyright (C) 2015 Nacho Fuentes Talens
Source file for Light class in NachosEngine
*/



#include "NachosEngine\light.h"

#include "GL\glew.h"

namespace NachosEngine {
  Light::Light() {
    view_ = glm::mat4();
    projection_ = glm::mat4();
  }

  Light::~Light() {

  }

  void Light::Update() {
    projection_ = glm::perspective(fovy_, aspect_ratio_, znear_, zfar_);
    view_ = glm::lookAt(position_, direction_, up_direction_);
  }

  void Light::DrawLightMap(Camera* camera) {
    frame_buffer_.Activate();
    frame_buffer_.AttachTexture(FrameBuffer::Attachment::kAttachment_Color, color_texture_);
    frame_buffer_.AttachTexture(FrameBuffer::Attachment::kAttachment_Depth, depth_texture_);

    //GPU Manager check frame buffer status
    //GPU Manager clearscreen
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Camera DrawLightMap(View,Projection)
    camera->DrawLightMap(view_,projection_);

    frame_buffer_.Deactivate();
  }

  //Projection
  void Light::set_fovy(const float fovy) {
    fovy_ = fovy;
  }
  void Light::set_aspect_ratio(const float aspect_ratio) {
    aspect_ratio_ = aspect_ratio;
  }
  void Light::set_znear(const float znear) {
    znear_ = znear;
  }
  void Light::set_zfar(const float zfar) {
    zfar_ = zfar;
  }

  //View
  void Light::set_position(const float x, const float y, const float z) {
    position_ = glm::vec3(x, y, z);
  }
  void Light::set_direction(const float x, const float y, const float z) {
    direction_ = glm::vec3(x, y, z);
  }
  void Light::set_up_direction(const float x, const float y, const float z) {
    up_direction_ = glm::vec3(x, y, z);
  }

  float* Light::view() {
    return glm::value_ptr(view_);
  }

  float* Light::projection() {
    return glm::value_ptr(projection_);
  }
} //end namespace NachosEngine