/*
Copyright (C) 2015 Nacho Fuentes Talens
Geometry Class
*/

#include <vector>

#include "NachosEngine\geometry.h"
#include "NachosEngine\geometry_builder.h"

#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "assimp\scene.h"
#include "assimp\mesh.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

namespace NachosEngine {

  Geometry::Geometry() {

    for (int i = 0; i < kAttributeName_Max; ++i) {
      mask_[i] = kAttributeType_None;
    }
    attribute_offsets_.alloc(kAttributeName_Max);

  }

  Geometry::~Geometry() {

  }

  void Geometry::set_data(const void* data,
    unsigned int size,
    unsigned int offset) {

    data_.init(size);
    data_.Upload(data,Buffer::kAttribute,size,offset);

  }

  void Geometry::set_elements(const void* data,
    unsigned int size,
    unsigned int offset) {

    elements_.init(size, Buffer::kElement);
    elements_.Upload(data, Buffer::kElement, size, offset);

  }

  void Geometry::set_mask(AttribMask mask) {

    for (int i = 0; i < kAttributeName_Max; ++i) {
      if(mask[i]>=0) {
      mask_[i] = mask[i];
      attribute_offsets_[i] = sizeof(float)*mask_[i];
      }
    }

  }

  Buffer* Geometry::data() {
    return &data_;
  }

  Buffer* Geometry::elements() {
    return &elements_;
  }

  void Geometry::set_geo_attrib_info(GeometryAttributeInformation geo_attrib_info[]) {

    for (unsigned int i = 0; i < kAttributeName_Max; ++i) {
      geo_attrib_info_[i] = geo_attrib_info[i];
    }

  }

  AttribType Geometry::geo_attrib_type(unsigned int idx) {
    return geo_attrib_info_[idx].attrib_type;
  }

  unsigned int Geometry::geo_attrib_offset(unsigned int idx) {
    return geo_attrib_info_[idx].offset;
  }

  void Geometry::set_elements_quantity(unsigned int quantity) {
    elements_quantity_ = quantity;
  }

  unsigned int Geometry::elements_quantity() {
    return elements_quantity_;
  }

  void Geometry::LoadGeometryFromFile(const char* file_to_load) {

    Assimp::Importer Importer;

    const aiScene *pScene = NULL;

    const aiMesh *pMesh = NULL;

    unsigned long int nVertices;

    glm::vec3 *positions;
    glm::vec3 *normals;

    glm::vec2 *uvs;

    NachosEngine::GeometryBuilder GB;
    
    pScene = Importer.ReadFile(file_to_load, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_ValidateDataStructure | aiProcess_FindInvalidData);

    if (!pScene) {
      printf("Error parsing '%s': '%s'\n", file_to_load, Importer.GetErrorString());
      return;
    }

    unsigned int nIndices;
    unsigned int *pIndices;

    pMesh = pScene->mMeshes[0];

    if (!pMesh) {
      printf("Error Finding Model In file.  Did you export an empty scene?");
      return;
    }

    nVertices = pMesh->mNumVertices;

    if (pMesh->HasFaces()) {
      GB.Begin(nVertices);

      aiFace *pAIFaces;

      pAIFaces = pMesh->mFaces;
      nIndices = pMesh->mNumFaces * 3;

      pIndices = new unsigned int[nIndices];

      for (unsigned long int i = 0; i < pMesh->mNumFaces; i++) {
        if (pAIFaces[i].mNumIndices != 3) {
          //aiReleaseImport(pScene);
          delete[] pIndices;
          return;
        }

        for (unsigned long int j = 0; j < 3; j++) {
          pIndices[i * 3 + j] = pAIFaces[i].mIndices[j];
        }
      }
    }

    if (pMesh->HasPositions()) {
      positions = new glm::vec3[nVertices];
            
      for (unsigned long int i = 0; i < nVertices; i++) {
        positions[i].x = pMesh->mVertices[i].x;
        positions[i].y = pMesh->mVertices[i].y;
        positions[i].z = pMesh->mVertices[i].z;
      }

      if (pMesh->HasNormals()) {
        normals = new glm::vec3[nVertices];
        for (unsigned long int i = 0; i < nVertices; i++) {
          normals[i].x = pMesh->mNormals[i].x;
          normals[i].y = pMesh->mNormals[i].y;
          normals[i].z = pMesh->mNormals[i].z;
        }
        GB.Add(NachosEngine::kAttributeName_Normal, NachosEngine::kAttributeType_Float3, &normals[0].x);
      }

      if (pMesh->HasTextureCoords(0)) {
        uvs = new glm::vec2[nVertices];
        for (unsigned long int i = 0; i < nVertices; i++) {
          uvs[i].x = 1-pMesh->mTextureCoords[0][i].x;
          uvs[i].y = pMesh->mTextureCoords[0][i].y;
        }
        GB.Add(NachosEngine::kAttributeName_UV, NachosEngine::kAttributeType_Float2, &uvs[0].x);
      }
      GB.Add(NachosEngine::kAttributeName_Position, NachosEngine::AttribType::kAttributeType_Float3, &positions[0].x);
    }
    
    GB.End(this, pIndices, nIndices);

  }

} //end namespace NachosEngine