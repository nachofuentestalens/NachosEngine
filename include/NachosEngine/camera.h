/*
Copyright (C) 2015 Nacho Fuentes Talens
Header file for Camera class in NachosEngine
*/

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "glm\glm.hpp"
#include "glm\ext.hpp"

#include "node.h"
#include "frame_buffer.h"
#include "drawable.h"
#include "material.h"

namespace NachosEngine {

  class Camera {
    public:

      struct RenderBinData {
        Geometry* geometry;
        Material* material;
        MaterialSettings* material_settings;
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 model;
      };

      Camera();
      ~Camera();

      glm::mat4 light_projection_;

      //Projection
      void set_fovy(const float fovy);
      void set_aspect_ratio(const float aspect_ratio);
      void set_znear(const float znear);
      void set_zfar(const float zfar);
      float* projection();

      //View
      void set_position(const float x, const float y, const float z);
      void set_direction(const float x, const float y, const float z);
      void set_up_direction(const float x, const float y, const float z);
      void set_root(ref_ptr<Node> root);
      float* view();

      //Light
      void CalculateLightDepth();
      float *light_view();

      unsigned int light_depth_texture();
	  unsigned int blurred_texture();

      void Draw();
      void DrawInWindow();
      void DrawInTexture(int level = 0);
      void DrawLightMap();
      void DrawLightMap(glm::mat4 light_view, glm::mat4 light_projection );
	  void DrawGlowObjects();

      void Move(const float x, const float y, const float z);
      void Rotate(const float amout);

      void Update();
    protected:


    private:
      //Projection
      float fovy_;
      float aspect_ratio_;
      float znear_;
      float zfar_;
      glm::mat4 projection_;
      //View
      glm::vec3 position_;
      glm::vec3 direction_;
      glm::vec3 up_direction_;
      glm::mat4 view_;
      //clase light
      glm::mat4 light_view_;
      
      std::vector<unsigned int> used_programs_;

      std::vector<RenderBinData> render_bin_;
	  std::vector<RenderBinData> glow_render_bin_;
      std::vector<ref_ptr<Node>> render_bin_by_mat_;

      ref_ptr<Node> root_;
      unsigned int color_texture_;
      unsigned int depth_texture_;
      //clase light
      //si tengo 15 luces pero solo soporto 10, calculo las 10 mas influyentes
      unsigned int light_color_texture_;
      unsigned int light_depth_texture_;
	  unsigned int glow_color_texture_;
	  unsigned int glow_depth_texture_;

	  unsigned int glow2_color_texture_;
	  unsigned int glow2_depth_texture_;
	  unsigned int glow3_color_texture_;
	  unsigned int glow3_depth_texture_;

      FrameBuffer frame_buffer_;
      FrameBuffer light_buffer_;
	  FrameBuffer glow_buffer_;
	  FrameBuffer glow2_buffer_;
	  FrameBuffer glow3_buffer_;

      ref_ptr<Geometry> quad;
      ref_ptr<Material> lightmap_material_;
      ref_ptr<MaterialSettings> lightmap_mats_;
      ref_ptr<NachosEngine::Material> quad_mat;
      ref_ptr<NachosEngine::MaterialSettings> quad_mats;
	  ref_ptr<Material> Vblur_material_;
	  ref_ptr<Material> Hblur_material_;
	  ref_ptr<NachosEngine::MaterialSettings> Vblur_mats_;
	  ref_ptr<NachosEngine::MaterialSettings> Hblur_mats_;

	  ref_ptr<Material>texture_mat_;
	  ref_ptr<MaterialSettings>texture_mats_;

	  ref_ptr<Material>glow_postprocess_mat_;
	  ref_ptr<MaterialSettings>glow_postprocess_mats_;


      ref_ptr<Drawable> drawable_quad_;
  };

} //end namespace NachosEngine

#endif //__CAMERA_H__