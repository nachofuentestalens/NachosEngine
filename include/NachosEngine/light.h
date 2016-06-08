/*
Copyright (C) 2015 Nacho Fuentes Talens
Header file for Light class in NachosEngine
*/

#ifndef __LIGHT_H__
#define __LIGHT_H__ 1

#include "referenced.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

#include "frame_buffer.h"
#include "camera.h"

namespace NachosEngine {
  class Light : public Referenced{
    public:
      Light();
      ~Light();

      void Update();
      void DrawLightMap(Camera* camera);

      //Projection
      void set_fovy(const float fovy);
      void set_aspect_ratio(const float aspect_ratio);
      void set_znear(const float znear);
      void set_zfar(const float zfar);
      //View
      void set_position(const float x, const float y, const float z);
      void set_direction(const float x, const float y, const float z);
      void set_up_direction(const float x, const float y, const float z);

      //Getters
      float* view();
      float* projection();

    protected:

    private:
      Light& operator=(const Light&){}
      Light(const Light&){}

      //View
      glm::vec3 position_;
      glm::vec3 direction_;
      glm::vec3 up_direction_;
      glm::mat4 view_;

      //Projection
      float fovy_;
      float aspect_ratio_;
      float znear_;
      float zfar_;
      glm::mat4 projection_;

      unsigned int color_texture_;
      unsigned int depth_texture_;

      bool casts_shadow_;
      FrameBuffer frame_buffer_;
  };
} //end namespace NachosEngine

#endif