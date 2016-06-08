/*
Copyright (C) 2015 Nacho Fuentes Talens
Node class
*/

#include "NachosEngine\node.h"

#define GLM_FORCE_RADIANS


namespace NachosEngine {

  struct Node::Data {
    glm::mat4 rot_;
    glm::mat4 tras_;
    glm::mat4 scale_;
    glm::mat4 model_;
    glm::mat4 parent_model_;
  };

  Node::Node(){
    id_ = 0;
    num_childs_ = 0;
    parent_ = NULL;
    name_[0] = '\0';
    data_ = new Data;
    data_->parent_model_ = glm::mat4(1.0f);
    data_->scale_ = glm::mat4(1.0f);
    data_->rot_ = glm::mat4(1.0f);
    data_->tras_ = glm::mat4(1.0f);
    data_->model_ = glm::mat4(1.0f);
  }

  Node::~Node() {

  }

  void Node::AddChild(Node* n) {

    if (n->parent_ != NULL) {
      n->parent_ = NULL;
    }

    childs_.push_back(n);

    n->id_ = num_childs_;

    childs_[num_childs_]->parent_ = this;

    num_childs_++;

  }

  void Node::RemoveChild(Node* n) {

    childs_[n->id_] = childs_.back();

    childs_.pop_back();

    num_childs_--;

    n->parent_ = NULL;

    data_->parent_model_ = glm::mat4(1.0f);

  }
  float* Node::model() {
    return glm::value_ptr(data_->model_);
  }

  glm::mat4 Node::model_as_mat() {
    return data_->model_;
  }

  void Node::set_position(const float positionx, const float positiony, const float positionz) {
    data_->tras_ = glm::translate(glm::mat4(1.0f), glm::vec3(positionx, positiony, positionz));
  }
  void Node::set_rotation(const float rotationx, const float rotationy, const float rotationz) {
    data_->rot_ = glm::rotate(glm::mat4(1.0f), rotationx, glm::vec3(1.0f, 0.0f, 0.0f)) *
      glm::rotate(glm::mat4(1.0f), rotationy, glm::vec3(0.0f, 1.0f, 0.0f)) *
      glm::rotate(glm::mat4(1.0f), rotationz, glm::vec3(0.0f, 0.0f, 1.0f));
  }

  void Node::set_scale(const float scalex, const float scaley, const float scalez) {
    data_->scale_ = glm::scale(glm::vec3(scalex, scaley, scalez));
  }

  void Node::set_scale(const float scale) {
    data_->scale_ = glm::scale(glm::vec3(scale, scale, scale));
  }

  void Node::Update() {

    data_->model_ = data_->scale_*data_->tras_*data_->rot_;

    for (unsigned int i = 0; i < childs_.size(); ++i) {
      childs_[i]->data_->parent_model_ = data_->model_;
      childs_[i]->Update();
    }

    if (parent_ != NULL) {
      data_->model_ = parent_->data_->model_ * data_->model_;
    }

  }

  void Node::Draw() {
    
  }

  std::vector<ref_ptr<Node>> Node::childs() {
    return childs_;
  }
} //end namespace NachosEngine