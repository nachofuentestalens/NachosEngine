/*
Copyright (C) 2015 Nacho Fuentes Talens
Header file for Window class in NachosEngine
*/

#ifndef WINDOW_H_
#define WINDOW_H_

namespace NachosEngine {
	class Window {
	public:
    struct Input_booleans {
      bool q;
      bool w;
      bool e;
      bool r;
      bool t;
      bool y;
      bool u;
      bool i;
      bool o;
      bool p;
      bool a;
      bool s;
      bool d;
      bool f;
      bool g;
      bool h;
      bool j;
      bool k;
      bool l;
      bool z;
      bool x;
      bool c;
      bool v;
      bool b;
      bool n;
      bool m;
      bool ESC;
      bool L_CTRL;
      bool R_CTRL;
      bool R_ALT;
      bool SPACE;
      bool L_MOUSE;
      bool R_MOUSE;
      bool four;
      bool six;
      bool dlt;
      bool L_Arrow;
      bool R_Arrow;
    };
		//constructor
		Window();
		//destructor
		~Window();

		bool init(int width, int height);
		bool processEvents();
    int width(){return width_;}
    int height(){return height_;}
    bool show_fps() {return show_fps_;}
    bool paused() { return paused_;}
    bool update_shaders(){return update_shaders_;}
    void not_update_shaders(){update_shaders_ = false;}
		bool swap();
		bool finish();
    Input_booleans input() {return input_;}
    int* mouse();
    void ShowCursor(bool show);
    void set_cursor(int x, int y);

    float speed = 3.0f; // 3 units / second
    float mouseSpeed = 0.005f;
    // horizontal angle : toward -Z
    float horizontalAngle;
    // vertical angle : 0, look at the horizon
    float verticalAngle;
    Input_booleans input_;
	private:
		struct Data;
		Data* data_;
    int width_;
    int height_;
		bool show_fps_;
    bool paused_;
    bool update_shaders_;
		 
	};

} //end namespace NachosEngine
#endif //WINDOW_H_