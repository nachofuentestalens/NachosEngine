/*
Copyright (C) 2015 Nacho Fuentes Talens
Header file for Node class in NachosEngine
*/

#ifndef __NODE_H__
#define __NODE_H__ 1

#include <vector>
#include "glm\glm.hpp"
#include "glm\ext.hpp"

#include "referenced.h"
#include "ref_ptr.h"

#include "geometry.h"
#include "material.h"
#include "material_settings.h"

namespace NachosEngine {
  class Node : public Referenced {
    public:
      Node();
      ~Node();

      char name_[100];
      float* model();
      glm::mat4 model_as_mat();

      void AddChild(Node* n);
      void RemoveChild(Node *n);
      
      void set_position(const float positionx, const float positiony, const float positionz);
      void set_rotation(const float rotationx, const float rotationy, const float rotationz);
      void set_scale(const float scalex, const float scaley, const float scalez);
      void set_scale(const float scale);

      virtual void Update();
      virtual void Draw();

      std::vector<ref_ptr<Node>> childs();
    protected:
      struct Data;
      Data* data_;

      Node* parent_;
      std::vector <ref_ptr<Node>> childs_;
      unsigned int id_;
      unsigned int num_childs_;

    private:
      Node(const Node&);
      Node& operator=(const Node &);

      
  };
}//end namespace NachosEngine

#endif //__NODE_H__

