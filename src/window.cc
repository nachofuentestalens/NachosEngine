/*
Copyright (C) 2015 Nacho Fuentes Talens
Window class c++ code
*/

#include "NachosEngine\window.h"

#include <iostream>

#include <SDL\SDL.h>
#include <GL\glew.h>


namespace NachosEngine {
  struct Window::Data {
    SDL_Window* sdl2_window;
    SDL_GLContext opengl3_context;
  };
  Window::Window(){
    data_ = new Data;
    show_fps_ = false;
    paused_ = false;
    update_shaders_ = false;
    bool q = false;
    bool w = false;
    bool e = false;
    bool r = false;
    bool t = false;
    bool y = false;
    bool u = false;
    bool i = false;
    bool o = false;
    bool p = false;
    bool a = false;
    bool s = false;
    bool d = false;
    bool f = false;
    bool g = false;
    bool h = false;
    bool j = false;
    bool k = false;
    bool l = false;
    bool z = false;
    bool x = false;
    bool c = false;
    bool v = false;
    bool b = false;
    bool n = false;
    bool m = false;
    bool ESC = false;
    bool L_CTRL = false;
    bool R_CTRL = false;
    bool SPACE = false;
    bool L_MOUSE = false;
    bool R_MOUSE = false;
    bool four = false;
    bool six = false;
    horizontalAngle = 3.14f;
    verticalAngle = 0.0f;
  }

  Window::~Window() {
    delete(data_);
  }

  bool Window::init(int width, int height) {
    width_ = width;
    height_ = height;
    SDL_Init(SDL_INIT_VIDEO);

    // set the opengl context version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    data_->sdl2_window = SDL_CreateWindow("NachosEngine", SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED, width, height,
      SDL_WINDOW_OPENGL);

    if (data_->sdl2_window == nullptr) {
      std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
      SDL_Quit();
      return false;
    }

    // create the opengl3 context
    data_->opengl3_context = SDL_GL_CreateContext(data_->sdl2_window);

    SDL_GL_MakeCurrent(data_->sdl2_window, data_->opengl3_context);

    //allow higher openGL versions
    glewExperimental = GL_TRUE;

    GLenum status = glewInit();
    if (status != GLEW_OK) {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      //exit(1);
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    //make our vertex array object, we need it to restore state we set after binding it. Re-binding reloads the state associated with it.
    glBindVertexArray(vao);

    glEnable(GL_DEPTH_TEST);
    //glCullFace(GL_FRONT_AND_BACK);
    //glFrontFace(GL_CCW);
    return true;

  }

  bool Window::processEvents() {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT: return false;

      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          input_.ESC = true;
          return false;
        }

        if (event.key.keysym.sym == SDLK_f) {
          show_fps_ = !show_fps_;
          input_.f = true;
        }

        if (event.key.keysym.sym == SDLK_c) input_.c = true;

        if (event.key.keysym.sym == SDLK_p)  {
          paused_ = !paused_;
          input_.p = true;
        }

        if (event.key.keysym.sym == SDLK_a) input_.a = true;
        if (event.key.keysym.sym == SDLK_b) input_.b = true;
        if (event.key.keysym.sym == SDLK_c) input_.c = true;
        if (event.key.keysym.sym == SDLK_d) input_.d = true;
        if (event.key.keysym.sym == SDLK_e) input_.e = true;
        if (event.key.keysym.sym == SDLK_f) input_.f = true;
        if (event.key.keysym.sym == SDLK_g) input_.g = true;
        if (event.key.keysym.sym == SDLK_h) input_.h = true;
        if (event.key.keysym.sym == SDLK_i) input_.i = true;
        if (event.key.keysym.sym == SDLK_j) input_.j = true;
        if (event.key.keysym.sym == SDLK_k) input_.k = true;
        if (event.key.keysym.sym == SDLK_l) input_.l = true;
        if (event.key.keysym.sym == SDLK_m) input_.m = true;
        if (event.key.keysym.sym == SDLK_n) input_.n = true;
        if (event.key.keysym.sym == SDLK_o) input_.o = true;
        if (event.key.keysym.sym == SDLK_p) input_.p = true;
        if (event.key.keysym.sym == SDLK_q) input_.q = true;
        if (event.key.keysym.sym == SDLK_r) input_.r = true;
        if (event.key.keysym.sym == SDLK_s) input_.s = true;
        if (event.key.keysym.sym == SDLK_t) input_.t = true;
        if (event.key.keysym.sym == SDLK_u) input_.u = true;
        if (event.key.keysym.sym == SDLK_v) input_.v = true;
        if (event.key.keysym.sym == SDLK_w) input_.w = true;
        if (event.key.keysym.sym == SDLK_x) input_.x = true;
        if (event.key.keysym.sym == SDLK_y) input_.y = true;
        if (event.key.keysym.sym == SDLK_z) input_.z = true;
        if (event.key.keysym.sym == SDLK_SPACE) input_.SPACE = true;
        if (event.key.keysym.sym == SDLK_LCTRL) input_.L_CTRL = true;
        if (event.key.keysym.sym == SDLK_RCTRL) input_.R_CTRL = true;
        if (event.key.keysym.sym == SDLK_RALT) input_.R_ALT = true;
        if (event.key.keysym.sym == SDLK_DELETE) input_.dlt = true;
        if (event.key.keysym.sym == SDLK_LEFT) input_.L_Arrow = true;
        if (event.key.keysym.sym == SDLK_RIGHT) input_.R_Arrow = true;
        if (event.key.keysym.sym == SDLK_KP_4) input_.four = true;
        if (event.key.keysym.sym == SDLK_KP_6) input_.six = true;
        break;

      case SDL_KEYUP:
        if (event.key.keysym.sym == SDLK_a) input_.a = false;
        if (event.key.keysym.sym == SDLK_b) input_.b = false;
        if (event.key.keysym.sym == SDLK_c) input_.c = false;
        if (event.key.keysym.sym == SDLK_d) input_.d = false;
        if (event.key.keysym.sym == SDLK_e) input_.e = false;
        if (event.key.keysym.sym == SDLK_f) input_.f = false;
        if (event.key.keysym.sym == SDLK_g) input_.g = false;
        if (event.key.keysym.sym == SDLK_h) input_.h = false;
        if (event.key.keysym.sym == SDLK_i) input_.i = false;
        if (event.key.keysym.sym == SDLK_j) input_.j = false;
        if (event.key.keysym.sym == SDLK_k) input_.k = false;
        if (event.key.keysym.sym == SDLK_l) input_.l = false;
        if (event.key.keysym.sym == SDLK_m) input_.m = false;
        if (event.key.keysym.sym == SDLK_n) input_.n = false;
        if (event.key.keysym.sym == SDLK_o) input_.o = false;
        if (event.key.keysym.sym == SDLK_p) input_.p = false;
        if (event.key.keysym.sym == SDLK_q) input_.q = false;
        if (event.key.keysym.sym == SDLK_r) input_.r = false;
        if (event.key.keysym.sym == SDLK_s) input_.s = false;
        if (event.key.keysym.sym == SDLK_t) input_.t = false;
        if (event.key.keysym.sym == SDLK_u) input_.u = false;
        if (event.key.keysym.sym == SDLK_v) input_.v = false;
        if (event.key.keysym.sym == SDLK_w) input_.w = false;
        if (event.key.keysym.sym == SDLK_x) input_.x = false;
        if (event.key.keysym.sym == SDLK_y) input_.y = false;
        if (event.key.keysym.sym == SDLK_z) input_.z = false;
        if (event.key.keysym.sym == SDLK_SPACE) input_.SPACE = false;
        if (event.key.keysym.sym == SDLK_LCTRL) input_.L_CTRL = false;
        if (event.key.keysym.sym == SDLK_RCTRL) input_.R_CTRL = false;
        if (event.key.keysym.sym == SDLK_RALT) input_.R_ALT = false;
        if (event.key.keysym.sym == SDLK_DELETE) input_.dlt = false;
        if (event.key.keysym.sym == SDLK_KP_4) input_.four = false;
        if (event.key.keysym.sym == SDLK_KP_6) input_.six = false;
        if (event.key.keysym.sym == SDLK_LEFT) input_.L_Arrow = false;
        if (event.key.keysym.sym == SDLK_RIGHT) input_.R_Arrow = false;
        break;

      case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_RIGHT) {
        input_.R_MOUSE = true;
        }

        if (event.button.button == SDL_BUTTON_LEFT) {
        input_.L_MOUSE = true;
        }
        break;

      case SDL_MOUSEBUTTONUP:
        if (event.button.button == SDL_BUTTON_RIGHT) {
        input_.R_MOUSE = false;
        }

        if (event.button.button == SDL_BUTTON_LEFT) {
        input_.L_MOUSE = false;
        }
        break;
      }
    }
    return true;

  }

  int* Window::mouse() {
    int mouse_position[2] = { 0, 0 };
    SDL_GetMouseState(&mouse_position[0], &mouse_position[1]);
    return mouse_position;
  }

  void Window::ShowCursor(bool show) {
    SDL_ShowCursor(show);
  }

  void Window::set_cursor(int x, int y) {
    SDL_WarpMouseInWindow(data_->sdl2_window,x,y);
  }


  bool Window::swap() {
    SDL_GL_SwapWindow(data_->sdl2_window);
    return true;
  }

  bool Window::finish() {
    SDL_GL_DeleteContext(data_->opengl3_context);
    SDL_DestroyWindow(data_->sdl2_window);
    SDL_Quit();
    return true;
  }
}




