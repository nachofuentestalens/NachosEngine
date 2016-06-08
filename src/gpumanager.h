/*
Copyright (C) 2015 Nacho Fuentes Talens
Header file with GPU_Manager class
*/

#ifndef __GPU_MANAGER_H__
#define __GPU_MANAGER_H__ 1


#include <vector>

#include "NachosEngine\geometry.h"
#include "NachosEngine\buffer.h"
#include "NachosEngine\material.h"
#include "NachosEngine\texture.h"
#include "NachosEngine\material_settings.h"
#include "NachosEngine\typedefs.h"

namespace NachosEngine {

  class GPU_Manager {
    public: 
      GPU_Manager();
      ~GPU_Manager();

      void Draw(Geometry *geometry, Material *material, MaterialSettings *materialsettings, Mode dmode);

    protected:

    private:
      int previous_program_;

  };
  
  extern GPU_Manager gpu_manager;

  struct Buffer::InternalB {

    unsigned int id_;
    unsigned int size_;

    unsigned int id(){ return id_; }

    void bind(const Target t);

    void init(const unsigned int size);

    void init(const unsigned int size, Target t);

    void Upload(const void* data,
      const Target t,
      const unsigned int size,
      const unsigned int offset = 0);
  };

  struct Material::InternalM {

    struct MaterialParameterIndex{
      //change name to scoped_array<char>
      char name_[40];
      DataType type_;
      unsigned int index_;
      unsigned int offset_;
      int size_;
    }; scoped_array<MaterialParameterIndex> MPI;


    unsigned int program_;
    int attribute_quantity_;
    int uniform_quantity_;
    int max_uniform_length_;
    int attribute_position_;
    int attribute_color_;
    AttribMask mask_;

    int uniform_quantity() { return uniform_quantity_; }
    unsigned int program(){ return program_; }
    MaterialParameterIndex mpi(unsigned int index) { return MPI[index]; }
    int attribute_position() { return attribute_position_; }
    int attribute_color() { return attribute_color_; }
    void CalculateMask();
    unsigned int LoadShader(const char* vs, const char* fs);
    void ReleaseMaterial();
    void _InternalGetUniforms();

  };

  struct MaterialSettings::InternalMS{

    scoped_array<float> data_;
    Material* mat_;
    std::vector<unsigned int> textures_;
    std::vector<std::string> sampler_names_;
    unsigned int num_textures_;

    void set_material(Material* mat);
    void UniformValue(const char* uniform, const float* data);
    void UniformValue(const char* uniform, const unsigned int* data);
    void AddTexture(const unsigned int id);
	unsigned int GetTexture();
	void OverwriteTexture0(const unsigned int new_text);
    void AddSamplerName(const char* name);
    void RemoveTexture(const unsigned int id);
    void RemoveSamplerName(const char* name);

    void _InternalAllocUniformData();
    void _InternalUploadUniforms();

  };

  struct Texture::InternalT {
    int textures_in_memory_;
    int max_texture_units_;

    struct TextureInformation {
      // # 8-bit components per pixel
      int width, height, components_per_pixel;
      unsigned char* data_;
      unsigned int id_;
      unsigned int texture_unit_;
      char name_[100];
    };
    std::vector<TextureInformation> textures_;

    //returns texture id
    unsigned int GenerateEmptyTexture(Format format, unsigned int width, unsigned int height);
    unsigned int LoadTexture(const char* texture);
    void ActivateTexture(const Material* mat, unsigned int id, const MaterialSettings* mats);
    void DeleteTexture(const unsigned int id);
  };
 
} //end namespace NachosEngine

#endif //__GPU_MANAGER_H__