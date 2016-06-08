/*
Copyright (C) 2015 Nacho Fuentes Talens
GPU_Manager class
*/

#include "gpumanager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"

#include <stddef.h>
#include <iostream>
#include <fstream>

#include "GL\glew.h"



namespace NachosEngine{
  
  GPU_Manager::GPU_Manager() {
    previous_program_ = -1;
  }

  GPU_Manager::~GPU_Manager() {
  }

  void GPU_Manager::Draw(Geometry *geometry, Material *material, MaterialSettings *materialsettings, Mode dmode) {
    
    unsigned int tmp_program = material->internal_material->program();
    if (tmp_program != previous_program_) {
      glUseProgram(material->internal_material->program());
    }
    unsigned int attrib_position = 0;
    for (int i = 0; i < kAttributeName_Max; ++i) {
      if ((geometry->mask(i) != material->internal_material->mask_[i]) && material->internal_material->mask_[i] != 0) {
        printf("\nThat geometry doesn't have the required attributes by that material\n");
      }
      geometry->data()->internal_buffer->bind(NachosEngine::Buffer::kAttribute);
      geometry->elements()->internal_buffer->bind(NachosEngine::Buffer::kElement);
      if (material->internal_material->mask_[i] != kAttributeType_None) {
        switch (material->internal_material->mask_[i]) {
        case kAttributeType_Float:
          glVertexAttribPointer(attrib_position, 1, GL_FLOAT, GL_FALSE, 0, (void*)geometry->geo_attrib_offset(i));
          break;
        case kAttributeType_Float2:
          glVertexAttribPointer(attrib_position, 2, GL_FLOAT, GL_FALSE, 0, (void*)geometry->geo_attrib_offset(i));
          break;
        case kAttributeType_Float3:
          glVertexAttribPointer(attrib_position, 3, GL_FLOAT, GL_FALSE, 0, (void*)geometry->geo_attrib_offset(i));
          break;
        case kAttributeType_Float4:
          glVertexAttribPointer(attrib_position, 4, GL_FLOAT, GL_FALSE, 0, (void*)geometry->geo_attrib_offset(i));
          break;
        }
        if (i == kAttributeName_UV) {
          for (unsigned int i = 0; i < materialsettings->internal_material_settings->textures_.size(); ++i) {
            Texture::Instance().internal_texture->ActivateTexture(material,
              materialsettings->internal_material_settings->textures_[i],
              materialsettings);
          }
        }
        glEnableVertexAttribArray(attrib_position);
        attrib_position++;
      }
    }

    materialsettings->internal_material_settings->_InternalUploadUniforms();
    GLenum glmode;
    switch (dmode) {
    case kPoints: glmode = GL_POINTS;
      break;
    case  kLineStrip: glmode = GL_LINE_STRIP;
      break;
    case kLineLoop: glmode = GL_LINE_LOOP;
      break;
    case kLines: glmode = GL_LINES;
      break;
    case kLineStripAdjency: glmode = GL_LINE_STRIP_ADJACENCY;
      break;
    case kLinesAdjency: glmode = GL_LINES_ADJACENCY;
      break;
    case kTriangleStrip: glmode = GL_TRIANGLE_STRIP;
      break;
    case kTriangleFan: glmode = GL_TRIANGLE_FAN;
      break;
    case kTriangles: glmode = GL_TRIANGLES;
      break;
    case kTriangleStripAdjency: glmode = GL_TRIANGLE_STRIP_ADJACENCY;
      break;
    case kTrianglesAdjency: glmode = GL_TRIANGLES_ADJACENCY;
      break;
    }
    unsigned int tmp = geometry->elements_quantity();
    glDrawElements(glmode, geometry->elements_quantity(), GL_UNSIGNED_INT, (void*)0);

    attrib_position = 0;
    for (int i = 0; i < kAttributeName_Max; ++i) {
      if (geometry->mask(i) != kAttributeType_None) {
        glDisableVertexAttribArray(attrib_position);
        attrib_position++;
      }
    }
    previous_program_ = material->internal_material->program();
  }

  //BUFFER
  Buffer::Buffer() {
    internal_buffer.alloc();
    internal_buffer->id_ = 0;
    internal_buffer->size_ = 0;
  }
  Buffer::~Buffer() {
    glDeleteBuffers(1, &internal_buffer->id_);
  }
  void Buffer::init(GLuint size) {
    internal_buffer->init(size);
  }

  void Buffer::init(GLuint size, Target t) {
    internal_buffer->init(size, t);
  }

  void Buffer::Upload(const void* data,
    const Target t,
    const unsigned int size,
    const unsigned int offset) {
    internal_buffer->Upload(data, t, size, offset);
  }
  
  void Buffer::InternalB::init(const GLuint size) {

    size_ = size;

    glGenBuffers(1, &id_);
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);

  }

  void Buffer::InternalB::init(const GLuint size, Target t) {

    size_ = size;

    glGenBuffers(1, &id_);

    switch (t) {
    case kAttribute:
      glBindBuffer(GL_ARRAY_BUFFER, id_);
      glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
      break;
    case kElement:
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
      break;
    }

  }

  void Buffer::InternalB::Upload(const void* data,
    const Target t,
    const unsigned int size,
    const unsigned int offset) {

    switch (t) {
    case kAttribute:
      glBindBuffer(GL_ARRAY_BUFFER, id_);
      glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
      break;
    case kElement:
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
      glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
      break;
    }

  }

  void Buffer::InternalB::bind(const Target t) {

    switch (t) {
    case kAttribute:
      glBindBuffer(GL_ARRAY_BUFFER, id_);
      break;
    case kElement:
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
      break;
    }

  }
  //FINISH BUFFER

  //MATERIAL
  void Material::InternalM::_InternalGetUniforms() {

    glGetProgramiv(program_, GL_ACTIVE_UNIFORMS, &uniform_quantity_);
    glGetProgramiv(program_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_uniform_length_);

    MPI.alloc(uniform_quantity_);

    for (int i = 0; i < uniform_quantity_; ++i) {
      GLsizei length;
      GLenum type;
      GLint size;

      glGetActiveUniform(program_, i, max_uniform_length_, &length, &size, &type, MPI[i].name_);

      switch (type) {
      case GL_FLOAT: MPI[i].type_ = FLOAT_1;
        MPI[i].size_ = sizeof(float);
        break;
      case GL_FLOAT_VEC2: MPI[i].type_ = FLOAT_2;
        MPI[i].size_ = sizeof(float) * 2;
        break;
      case GL_FLOAT_VEC3: MPI[i].type_ = FLOAT_3;
        MPI[i].size_ = sizeof(float) * 3;
        break;
      case GL_FLOAT_VEC4: MPI[i].type_ = FLOAT_4;
        MPI[i].size_ = sizeof(float) * 4;
        break;
      case GL_FLOAT_MAT4: MPI[i].type_ = MAT4x4;
        MPI[i].size_ = sizeof(float) * 16;
        break;
      case GL_SAMPLER_2D: MPI[i].type_ = SAMPLER_2D;
        MPI[i].size_ = sizeof(int);
      }

      MPI[i].index_ = glGetUniformLocation(program_, MPI[i].name_);

      if (i == 0) MPI[0].offset_ = 0;
      for (int i = 1; i < uniform_quantity_; ++i) {
        MPI[i].offset_ = MPI[i - 1].size_ + MPI[i - 1].offset_;
        float tmp_offset = MPI[i].offset_;
        int asd = 0;
      }
    }

  }

  void Material::InternalM::CalculateMask() {

    int max_attrib_lenght;

    glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTES, &attribute_quantity_);

    glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &max_attrib_lenght);

    for (int i = 0; i < attribute_quantity_; ++i) {
      GLsizei length;
      GLenum type;
      GLint size;

      char name[255];
      memset(name, 0, 255);

      int internal_type;

      glGetActiveAttrib(program_, i, max_attrib_lenght, &length, &size, &type, name);

      switch (type) {
      case GL_FLOAT: internal_type = kAttributeType_Float;
        break;
      case GL_FLOAT_VEC2: internal_type = kAttributeType_Float2;
        break;
      case GL_FLOAT_VEC3: internal_type = kAttributeType_Float3;
        break;
      case GL_FLOAT_VEC4: internal_type = kAttributeType_Float4;
        break;
      }

      if (strcmp(name, "a_position") == 0) {
        mask_[kAttributeName_Position] = (AttribType)internal_type;
      }
      else if (strcmp(name, "a_color") == 0) {
        mask_[kAttributeName_Color] = (AttribType)internal_type;
      }
      else if (strcmp(name, "a_uv") == 0) {
        mask_[kAttributeName_UV] = (AttribType)internal_type;
      }
      else if (strcmp(name, "a_normal") == 0) {
        mask_[kAttributeName_Normal] = (AttribType)internal_type;
      }
    }

  }

  unsigned int CreateShader(const GLenum eShaderType, const char *strShaderFile) {

    std::string vertex_code;

    std::ifstream vertex_file(strShaderFile, std::ifstream::in);

    char c = vertex_file.get();

    while (vertex_file.good()){
      vertex_code += c;
      c = vertex_file.get();
    }

    vertex_file.close();

    //display to make sure the string is loaded
    // std::cout << "\nVertex Code:\n" << vertex_code.c_str() << std::endl; 

    unsigned int shader = glCreateShader(eShaderType);

    const char *ss = vertex_code.c_str();;

    glShaderSource(shader, 1, &ss, NULL);

    glCompileShader(shader);

    printf("glError: %d\n", glGetError());

    GLint status;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
      GLint infoLogLength;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
      GLchar strInfoLog[4096];
      glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
      char strShaderType[16];
      switch (eShaderType) {
      case GL_VERTEX_SHADER: sprintf(strShaderType, "vertex"); break;
      case GL_FRAGMENT_SHADER: sprintf(strShaderType, "fragment"); break;
      case GL_GEOMETRY_SHADER: sprintf(strShaderType, "geometry"); break;
      }
      printf("\nCompile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
      return -1;
    }
    else {
      printf("\nShader compiled successfully\n");
      printf("glError: %d\n", glGetError());
      return shader;
    }

  }

  unsigned int Material::InternalM::LoadShader(const char *vs, const char *fs) {

    unsigned int vertexShader;
    unsigned int fragmentShader;

    vertexShader = CreateShader(GL_VERTEX_SHADER, vs);
    fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fs);

    program_ = glCreateProgram();

    glAttachShader(program_, vertexShader);
    glAttachShader(program_, fragmentShader);

    glLinkProgram(program_);

    //check errors
    GLint status;

    glGetProgramiv(program_, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
      GLint infoLogLength;
      glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &infoLogLength);

      GLchar strInfoLog[4096];
      glGetProgramInfoLog(program_, infoLogLength, NULL, strInfoLog);
      printf("\nShader linker failure: %s %s %s\n", strInfoLog, vs, fs);
      return -1;
    }
    else
      printf("\nShader linked sucessfully!\n");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    _InternalGetUniforms();
    CalculateMask();

    return program_;

  }

  void Material::InternalM::ReleaseMaterial() {
    glDeleteProgram(program_);
  }
  //FINISH MATERIAL

  //MaterialSettings
  void MaterialSettings::InternalMS::set_material(Material* mat) {
    mat_ = mat;
    _InternalAllocUniformData();
  }

  void MaterialSettings::InternalMS::UniformValue(const char* uniform, const float* data) {

    int mpi_index = -1;

    for (int i = 0; i < mat_->internal_material->uniform_quantity(); ++i) {
      if (strcmp(mat_->internal_material->mpi(i).name_, uniform) == 0) {
        mpi_index = i;
      }
    }

    switch (mat_->internal_material->mpi(mpi_index).type_) {
    case Material::DataType::FLOAT_1: data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float)] = data[0];
      break;

    case Material::DataType::FLOAT_2: data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float)] = data[0];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 1] = data[1];
      break;

    case Material::DataType::FLOAT_3: data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float)] = data[0];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 1] = data[1];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 2] = data[2];
      break;

    case Material::DataType::FLOAT_4: data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float)] = data[0];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 1] = data[1];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 2] = data[2];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 3] = data[3];
      break;

    case Material::DataType::MAT4x4: data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float)] = data[0];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 1] = data[1];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 2] = data[2];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 3] = data[3];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 4] = data[4];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 5] = data[5];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 6] = data[6];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 7] = data[7];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 8] = data[8];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 9] = data[9];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 10] = data[10];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 11] = data[11];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 12] = data[12];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 13] = data[13];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 14] = data[14];
      data_[(mat_->internal_material->mpi(mpi_index).offset_) / sizeof(float) + 15] = data[15];
      break;
    }

  }

  //TODO
  void MaterialSettings::InternalMS::UniformValue(const char* uniform, const unsigned int* data) {
    //TODO
  }

  void MaterialSettings::InternalMS::_InternalAllocUniformData() {

    if (mat_->internal_material->uniform_quantity() > 0) {
      data_.alloc((mat_->internal_material->mpi(mat_->internal_material->uniform_quantity() - 1).size_
        + mat_->internal_material->mpi(mat_->internal_material->uniform_quantity() - 1).offset_) / sizeof(float));
    }

  }

  void MaterialSettings::InternalMS::_InternalUploadUniforms() {
    for (int i = 0; i < mat_->internal_material->uniform_quantity(); ++i) {
      switch (mat_->internal_material->mpi(i).type_) {
      case Material::DataType::FLOAT_1: glUniform1f(mat_->internal_material->mpi(i).index_, data_[mat_->internal_material->mpi(i).offset_ / sizeof(float)]);
        break;

      case Material::DataType::FLOAT_2: glUniform2f(mat_->internal_material->mpi(i).index_, data_[mat_->internal_material->mpi(i).offset_ / sizeof(float)],
        data_[mat_->internal_material->mpi(i).offset_ / sizeof(float) + 1]);
        break;

      case Material::DataType::FLOAT_3: glUniform3f(mat_->internal_material->mpi(i).index_, data_[mat_->internal_material->mpi(i).offset_ / sizeof(float)],
        data_[mat_->internal_material->mpi(i).offset_ / sizeof(float) + 1], data_[mat_->internal_material->mpi(i).offset_ / sizeof(float) + 2]);
        break;

      case Material::DataType::FLOAT_4: glUniform4f(mat_->internal_material->mpi(i).index_, data_[mat_->internal_material->mpi(i).offset_ / sizeof(float)],
        data_[mat_->internal_material->mpi(i).offset_ / sizeof(float) + 1], data_[mat_->internal_material->mpi(i).offset_ / sizeof(float) + 2],
        data_[mat_->internal_material->mpi(i).offset_ / sizeof(float) + 3]);
        break;

      case Material::DataType::MAT4x4:
        glUniformMatrix4fv(mat_->internal_material->mpi(i).index_, 1, GL_FALSE, &data_[mat_->internal_material->mpi(i).offset_ / sizeof(float)]);
        break;
      }
    }
  }

  void MaterialSettings::InternalMS::AddTexture(const unsigned int id) {
    textures_.push_back(id);
    num_textures_++;
  }

  unsigned int MaterialSettings::InternalMS::GetTexture() {
	  return textures_[0];
  }

  void MaterialSettings::InternalMS::OverwriteTexture0(const unsigned int new_text) {
	  textures_[0] = new_text;
  }

  void MaterialSettings::InternalMS::AddSamplerName(const char* name) {
    std::string tmp = std::string(name);
    sampler_names_.push_back(tmp);
  }

  void MaterialSettings::InternalMS::RemoveTexture(const unsigned int id) {

    for (unsigned int i = 0; i < textures_.size(); ++i) {
      if (textures_[i] == id) {
        textures_[i] = textures_.back();
        textures_.pop_back();
        num_textures_--;
        return;
      }
    }

  }

  void MaterialSettings::InternalMS::RemoveSamplerName(const char* name) {

    std::string tmp = std::string(name);

    for (unsigned int i = 0; i < sampler_names_.size(); ++i) {
      if (sampler_names_[i] == tmp) {
        sampler_names_.back() = sampler_names_[i];
        sampler_names_.pop_back();
        return;
      }
    }

  }
  //Finish MaterialSettings

  //Texture
  unsigned int Texture::InternalT::LoadTexture(const char* texture) {

    assert(textures_in_memory_ < max_texture_units_ && "Too many textures, implement reordering of texture units");

    textures_.resize(textures_.size() + 1);

    glGenTextures(1, &textures_[textures_in_memory_].id_);

    textures_[textures_in_memory_].data_ = stbi_load(texture, &textures_[textures_in_memory_].width,
      &textures_[textures_in_memory_].height,
      &textures_[textures_in_memory_].components_per_pixel, 0);
    strcpy_s(textures_[textures_in_memory_].name_, texture);

    textures_[textures_in_memory_].texture_unit_ = textures_in_memory_;

    glActiveTexture(GL_TEXTURE0 + textures_[textures_in_memory_].texture_unit_);

    glBindTexture(GL_TEXTURE_2D, textures_[textures_in_memory_].id_);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures_[textures_in_memory_].width,
      textures_[textures_in_memory_].height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
      textures_[textures_in_memory_].data_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
    textures_in_memory_++;

    return textures_[textures_in_memory_ - 1].id_;

  }

  unsigned int Texture::InternalT::GenerateEmptyTexture(Format format, unsigned int width, unsigned int height) {

    assert(textures_in_memory_ < max_texture_units_ && "Too many textures, implement reordering of texture units");

    GLenum internal_format;
    GLenum type;

    switch (format) {
    case kFormat_RGB:
      internal_format = GL_RGB;
      type = GL_UNSIGNED_BYTE;
      break;
    case kFormat_DepthComponent:
      internal_format = GL_DEPTH_COMPONENT;
      type = GL_FLOAT;
      break;
    case kFormat_DepthSenthil:
      internal_format = GL_DEPTH_STENCIL;
      type = GL_FLOAT_VEC2;
      break;
    }

    textures_.resize(textures_.size() + 1);

    glGenTextures(1, &textures_[textures_in_memory_].id_);

    textures_[textures_in_memory_].texture_unit_ = textures_in_memory_;
    textures_[textures_in_memory_].width = width;
    textures_[textures_in_memory_].height = height;

    glActiveTexture(GL_TEXTURE0 + textures_[textures_in_memory_].texture_unit_);

    glBindTexture(GL_TEXTURE_2D, textures_[textures_in_memory_].id_);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, textures_[textures_in_memory_].width,
      textures_[textures_in_memory_].height, 0, internal_format, type,
      0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
      GL_TEXTURE_WRAP_S,
      GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		GL_CLAMP);
    glGenerateMipmap(GL_TEXTURE_2D);

    textures_in_memory_++;

    return textures_[textures_in_memory_ - 1].id_;

  }

  void Texture::InternalT::ActivateTexture(const Material* mat, unsigned int id, const MaterialSettings* mats) {

    //debug this. it's not working right
    unsigned int sampler_name;

    for (unsigned int i = 0; i < mats->internal_material_settings->sampler_names_.size(); ++i) {
      if (mats->internal_material_settings->textures_[i] == id) {
        sampler_name = i;
        break;
      }
    }

    for (unsigned int i = 0; i < textures_.size(); ++i) {
      if (textures_[i].id_ == id) {
        glUniform1i(glGetUniformLocation(mat->internal_material.get()->program(),
          mats->internal_material_settings->sampler_names_[sampler_name].c_str()),
          textures_[i].texture_unit_);
      }
    }

  }

  void Texture::InternalT::DeleteTexture(const unsigned int id) {

    for (unsigned int i = 0; i < textures_.size(); ++i) {
      if (textures_[i].id_ == id) {
        textures_[i] = textures_.back();
        textures_.pop_back();
        glDeleteTextures(1, &id);
        textures_in_memory_--;
        return;
      }
    }

  }
  //Finish Texture

}//end namespace NachosEngine