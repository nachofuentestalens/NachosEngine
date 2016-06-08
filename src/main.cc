
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "GL\glew.h"
#include <Windows.h>

#include "NachosEngine\window.h"
#include "NachosEngine\material.h"
#include "NachosEngine\buffer.h"
#include "NachosEngine\material_settings.h"
#include "NachosEngine\geometry.h"
#include "NachosEngine\ref_ptr.h"
#include "NachosEngine\texture.h"
#include "NachosEngine\camera.h"
#include "NachosEngine\geometry_builder.h"
#include "NachosEngine\drawable.h"
#include "NachosEngine\node.h"
#include "NachosEngine\light.h"



struct Timer {
	LARGE_INTEGER  Frequency;
	LARGE_INTEGER  StartingTime, EndingTime, ElapsedMicroseconds;
};

//Update Timer
Timer update_timer;

//Profiling Timer
Timer profiling_timer;

static void StartTimer(Timer &timer) {
	QueryPerformanceFrequency(&timer.Frequency);
	QueryPerformanceCounter(&timer.StartingTime);
}

static double GetTime(Timer &timer) {
	QueryPerformanceCounter(&timer.EndingTime);
	timer.ElapsedMicroseconds.QuadPart = timer.EndingTime.QuadPart - timer.StartingTime.QuadPart;
	//
	// We now have the elapsed number of ticks, along with the
	// number of ticks-per-second. We use these values
	// to convert to the number of elapsed microseconds.
	// To guard against loss-of-precision, we convert
	// to microseconds *before* dividing by ticks-per-second.
	//
	timer.ElapsedMicroseconds.QuadPart *= 1000000;
	double time = (double)(timer.ElapsedMicroseconds.QuadPart /= timer.Frequency.QuadPart);
	return time / 1000;
}

#define KEY1 0x01
#define KEY2 0x02

struct{
	float player1_pos[3];
	float player2_pos[3];
	short player1_keys;
	short player2_keys;
} game;

const float kPlayerOneStartingPosition[3] = { -8.0f, 0.5f, -2.0f };
const float kPlayerTwoStartingPosition[3] = { -8.0f, 0.5f, 2.0f };
const float kPlayerSpeed = 0.30f;
const float kFinishPoint = 9.0f;

void RestartGame(NachosEngine::Camera* camera) {
	game.player1_pos[0] = kPlayerOneStartingPosition[0];
	game.player1_pos[1] = kPlayerOneStartingPosition[1];
	game.player1_pos[2] = kPlayerOneStartingPosition[2];
	game.player2_pos[0] = kPlayerTwoStartingPosition[0];
	game.player2_pos[1] = kPlayerTwoStartingPosition[1];
	game.player2_pos[2] = kPlayerTwoStartingPosition[2];
	camera->set_fovy(90.0f);
	camera->set_aspect_ratio(8.0f / 6.0f);
	camera->set_znear(1.0f);
	camera->set_zfar(200.0f);
	camera->set_position(0.0f, 5.0f, 5.0f);
	camera->set_direction(0.0f, 0.0f, 0.0f);
	camera->set_up_direction(0.0f, 1.0f, 0.0f);
	camera->Move(-30.0f, 10.0f, 20.0f);
	camera->Update();
}

int main(int argc, char** argv) {

	game.player1_keys = 0;
	game.player2_keys = 0;
	game.player1_pos[0] = kPlayerOneStartingPosition[0];
	game.player1_pos[1] = kPlayerOneStartingPosition[1];
	game.player1_pos[2] = kPlayerOneStartingPosition[2];

	game.player2_pos[0] = kPlayerTwoStartingPosition[0];
	game.player2_pos[1] = kPlayerTwoStartingPosition[1];
	game.player2_pos[2] = kPlayerTwoStartingPosition[2];

	NachosEngine::Window w;
	if (!w.init(800, 600)) return 1;

	NachosEngine::Camera camera;

	NachosEngine::ref_ptr<NachosEngine::Light> light1;
	light1.allocT<NachosEngine::Light>();

	NachosEngine::ref_ptr<NachosEngine::Material> mat;
	NachosEngine::ref_ptr<NachosEngine::Material> mat2;
	NachosEngine::ref_ptr<NachosEngine::Material> mat3;

	NachosEngine::ref_ptr<NachosEngine::MaterialSettings> mats;
	NachosEngine::ref_ptr<NachosEngine::MaterialSettings> mats2;
	NachosEngine::ref_ptr<NachosEngine::MaterialSettings> mats3;
	NachosEngine::ref_ptr<NachosEngine::MaterialSettings> mats4;

	NachosEngine::ref_ptr<NachosEngine::Node> root;
	root.allocT<NachosEngine::Node>();

	NachosEngine::ref_ptr<NachosEngine::Drawable> drawable;
	NachosEngine::ref_ptr<NachosEngine::Drawable> drawable2;
	NachosEngine::ref_ptr<NachosEngine::Drawable> drawable_plane;
	NachosEngine::ref_ptr<NachosEngine::Drawable> drawable_green_light;

	drawable.allocT<NachosEngine::Drawable>();
	drawable2.allocT<NachosEngine::Drawable>();
	drawable_plane.allocT<NachosEngine::Drawable>();
	drawable_green_light.allocT<NachosEngine::Drawable>();

	root->AddChild(drawable.get());
	root->AddChild(drawable2.get());
	root->AddChild(drawable_green_light.get());
	root->AddChild(drawable_plane.get());
	root->set_position(0.0f, 0.0f, 0.0f);

	unsigned int uiTimeStep = 16;

	const float camera_speed = 0.1f;
	const char simple_vertex_shader[] = "Shaders/simple_vertex_shader.txt";
	const char simple_fragment_shader[] = "Shaders/simple_fragment_shader.txt";
	const char vertex_shader_textured[] = "Shaders/vertex_shader_textures.txt";
	const char fragment_shader_textured[] = "Shaders/fragment_shader_textures.txt";
	const char plane_vertex_shader[] = "Shaders/plane_vertex_shader.txt";
	const char plane_fragment_shader[] = "Shaders/plane_fragment_shader.txt";
	const char vertex_shader_shadow[] = "Shaders/shadow_mapping_vertex_shader.txt";
	const char fragment_shader_shadow[] = "Shaders/shadow_mapping_fragment_shader.txt";

	//Simple material
	mat.allocT<NachosEngine::Material>();
	mat->LoadShader(vertex_shader_shadow, fragment_shader_shadow);
	//Textured Material
	mat2.allocT<NachosEngine::Material>();
	mat2->LoadShader(vertex_shader_shadow, fragment_shader_shadow);
	//Textured Material with Shadows
	mat3.allocT<NachosEngine::Material>();
	mat3->LoadShader(vertex_shader_shadow, fragment_shader_shadow);



	mats.allocT<NachosEngine::MaterialSettings>();
	mats->set_material(mat3.get());
	mats2.allocT<NachosEngine::MaterialSettings>();
	mats2->set_material(mat3.get());
	mats3.allocT<NachosEngine::MaterialSettings>();
	mats3->set_material(mat2.get());
	mats4.allocT<NachosEngine::MaterialSettings>();
	mats4->set_material(mat3.get());

	unsigned int green_light_texture = NachosEngine::Texture::Instance().LoadTexture("Textures/green_light.png");
	unsigned int gustavo = NachosEngine::Texture::Instance().LoadTexture("Textures/gustavo.png");
	unsigned int jl = NachosEngine::Texture::Instance().LoadTexture("Textures/JL.png");
	unsigned int field = NachosEngine::Texture::Instance().LoadTexture("Textures/field2.png");

	mats->AddTexture(gustavo);
	mats->AddTexture(camera.light_depth_texture());
	mats->AddTexture(camera.blurred_texture());
	mats->AddSamplerName("tex");
	mats->AddSamplerName("light_depth");
	mats->AddSamplerName("blurred_texture");

	mats2->AddTexture(jl);
	mats2->AddTexture(camera.light_depth_texture());
	mats2->AddTexture(camera.blurred_texture());
	mats2->AddSamplerName("tex");
	mats2->AddSamplerName("light_depth");
	mats2->AddSamplerName("blurred_texture");

	mats3->AddTexture(field);
	mats3->AddTexture(camera.light_depth_texture());
	mats3->AddTexture(camera.blurred_texture());
	mats3->AddSamplerName("tex");
	mats3->AddSamplerName("light_depth");
	mats3->AddSamplerName("blurred_texture");


	mats4->AddTexture(green_light_texture);
	mats4->AddTexture(camera.light_depth_texture());
	mats4->AddTexture(camera.blurred_texture());
	mats4->AddSamplerName("tex");
	mats4->AddSamplerName("light_depth");
	mats4->AddSamplerName("blurred_texture");


	unsigned int frames_count = 0;
	float rot = 0.0f;

	camera.set_fovy(90.0f);
	camera.set_aspect_ratio(8.0f / 6.0f);
	camera.set_znear(1.0f);
	camera.set_zfar(200.0f);
	camera.set_position(0.0f, 5.0f, 5.0f);
	camera.set_direction(0.0f, 0.0f, 0.0f);
	camera.set_up_direction(0.0f, 1.0f, 0.0f);
	camera.set_root(root);
	camera.Move(-30.0f, 10.0f, 20.0f);
	camera.Update();

	light1->set_fovy(90.0f);
	light1->set_aspect_ratio(8.0f / 6.0f);
	light1->set_znear(1.0f);
	light1->set_zfar(200.0f);
	light1->set_position(0.0f, 10.0f, 15.0f);
	light1->set_direction(0.0f, 0.0f, 0.0f);
	light1->set_up_direction(0.0f, 1.0f, 0.0f);
	light1->Update();

	StartTimer(update_timer);
	double CurrentTime = GetTime(update_timer);

	NachosEngine::ref_ptr<NachosEngine::Geometry> geometry_plane;
	geometry_plane.allocT<NachosEngine::Geometry>();
	NachosEngine::GeometryBuilder tmp_gb;

	float plane_positions[12] = {
		-1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 1.0f
	};
	float plane_uvs[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	float plane_normals[12] = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	unsigned int plane_elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	tmp_gb.Begin(4);
	tmp_gb.Add(NachosEngine::kAttributeName_Position, NachosEngine::kAttributeType_Float3, plane_positions);
	tmp_gb.Add(NachosEngine::kAttributeName_UV, NachosEngine::kAttributeType_Float2, plane_uvs);
	tmp_gb.Add(NachosEngine::kAttributeName_Normal, NachosEngine::kAttributeType_Float3, plane_normals);
	tmp_gb.End(geometry_plane.get(), plane_elements, 6);

	NachosEngine::ref_ptr<NachosEngine::Geometry> Geo1;
	Geo1.allocT<NachosEngine::Geometry>();
	Geo1->LoadGeometryFromFile("Objs/cube1x1_t2.obj");

	NachosEngine::ref_ptr<NachosEngine::Geometry> Geo2;
	Geo2.allocT<NachosEngine::Geometry>();
	Geo2->LoadGeometryFromFile("Objs/sphere_2_t.obj");

	drawable_plane->set_geometry(geometry_plane.get());
	drawable_plane->set_material(mat2.get());
	drawable_plane->set_materialsettings(mats3.get());
	drawable_plane->set_position(0.0f, -0.1f, 0.0f);
	drawable_plane->set_scale(40.0f, 20.0f, 20.0f);
	//drawable_plane->postprocess_ = 0x01;

	drawable->set_geometry(geometry_plane.get());
	drawable->set_material(mat2.get());
	drawable->set_materialsettings(mats.get());
	drawable->set_position(game.player1_pos[0], game.player1_pos[1], game.player1_pos[2]);
	drawable->set_scale(4.0f);
	drawable->set_rotation(90.0f, 0.0f, 0.0f);
	//drawable->postprocess_ = 0x01;

	drawable2->set_geometry(geometry_plane.get());
	drawable2->set_material(mat2.get());
	drawable2->set_materialsettings(mats2.get());
	drawable2->set_position(game.player2_pos[0], game.player2_pos[1], game.player2_pos[2]);
	drawable2->set_scale(4.0f);
	drawable2->set_rotation(90.0f, 0.0f, 0.0f);
	//drawable2->postprocess_ = 0x01;

	
	drawable_green_light->set_geometry(Geo2.get());
	drawable_green_light->set_material(mat3.get());
	drawable_green_light->set_materialsettings(mats4.get());
	drawable_green_light->set_position(game.player1_pos[0],0.0f,-8.0f);
	drawable_green_light->set_scale(3.0f);
	drawable_green_light->postprocess_ = 0x01;

	bool bfirst_R_click = true;
	int ifirst_R_click[2] = { 0, 0 };

	//Calculate Light for the first time in initial position of the camera
	root->Update();
	camera.Update();


	camera.CalculateLightDepth();

	while (w.processEvents()) {
		if (w.input().r == true) {
			//Save in each mat the shader used, then reload that shader.
			mat->LoadShader(vertex_shader_shadow, fragment_shader_shadow);
			mat2->LoadShader(vertex_shader_shadow, fragment_shader_shadow);
			mat3->LoadShader(vertex_shader_shadow, fragment_shader_shadow);
		}
		if (!w.paused()) {
			//update...
			double accumTime = GetTime(update_timer) - CurrentTime;

			while (accumTime >= uiTimeStep) {
				if (w.input().SPACE == true){
					camera.Move(0.0f, camera_speed, 0.0f);
				}
				if (w.input().L_CTRL == true){
					camera.Move(0.0f, -camera_speed, 0.0f);
				}
				if (w.input().l == true) {
					camera.CalculateLightDepth();
				}
				if (w.input().a == true){
					camera.Move(-camera_speed, 0.0f, 0.0f);
				}
				if (w.input().d == true){
					camera.Move(camera_speed, 0.0f, 0.0f);
				}
				if (w.input().w == true){
					camera.Move(0.0f, 0.0f, -camera_speed);
				}
				if (w.input().s == true){
					camera.Move(0.0f, 0.0f, camera_speed);
				}

				if (w.input().R_MOUSE == true) {
					if (bfirst_R_click == true) {
						ifirst_R_click[0] = w.mouse()[0];
						ifirst_R_click[1] = w.mouse()[1];
						bfirst_R_click = false;
					}
					w.ShowCursor(false);
					int mouse_pos[2] = { 0, 0 };
					mouse_pos[0] = w.mouse()[0];
					mouse_pos[1] = w.mouse()[1];
				}
				if (w.input().R_MOUSE == false) {
					bfirst_R_click = true;
					w.ShowCursor(true);
				}

				if (w.input().dlt == true) {
					RestartGame(&camera);
				}


				camera.Update();
				CurrentTime += uiTimeStep;
				accumTime = GetTime(update_timer) - CurrentTime;


				root->Update();


			}

			if (w.input().q == true) {
				if (((game.player1_keys & KEY2) == KEY2) || game.player1_keys == 0) {
					game.player1_pos[0] += kPlayerSpeed;
					camera.Move(kPlayerSpeed*2.5f, 0.0f, 0.0f);
					game.player1_keys = KEY1;
					w.input_.q = false;
				}
			}
			if (w.input().e == true) {
				if (((game.player1_keys & KEY1) == KEY1) || game.player1_keys == 0) {
					game.player1_pos[0] += kPlayerSpeed;
					game.player1_keys = KEY2;
					w.input_.e = false;
					camera.Move(kPlayerSpeed*2.5f, 0.0f, 0.0f);
				}
			}

			if (w.input().L_Arrow == true) {
				if (((game.player2_keys & KEY2) == KEY2) || game.player2_keys == 0) {
					game.player2_pos[0] += kPlayerSpeed;
					game.player2_keys = KEY1;
					w.input_.L_Arrow = false;
				}
			}
			if (w.input().R_Arrow == true) {
				if (((game.player2_keys & KEY1) == KEY1) || game.player2_keys == 0) {
					game.player2_pos[0] += kPlayerSpeed;
					game.player2_keys = KEY2;
					w.input_.R_Arrow = false;
				}
			}

			drawable->set_position(game.player1_pos[0], game.player1_pos[1], game.player1_pos[2]);
			drawable2->set_position(game.player2_pos[0], game.player2_pos[1], game.player2_pos[2]);

			if (game.player1_pos[0] >= kFinishPoint) {
				printf("Player 1 wins!\n");
				RestartGame(&camera);
			}
			else if (game.player2_pos[0] >= kFinishPoint){
				printf("Player 2 wins!\n");
				RestartGame(&camera);
			}

			StartTimer(profiling_timer);
			camera.DrawInWindow();
			w.swap();

			double fps = 1000.0 / GetTime(profiling_timer);
			frames_count++;
			if (frames_count > 10 && w.show_fps()) {
				printf("FPS: %f \n", fps);
				frames_count = 0;
			}
		}
		if (w.paused()) {
			StartTimer(update_timer);
			CurrentTime = GetTime(update_timer);
		}
	}
	w.finish();
	return 0;
}