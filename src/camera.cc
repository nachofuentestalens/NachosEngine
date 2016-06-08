/*
Copyright (C) 2015 Nacho Fuentes Talens
Camera Class
*/

#include "NachosEngine\camera.h"

#include "GL\glew.h"

#include "NachosEngine\window.h"
#include "NachosEngine\drawable.h"
#include "NachosEngine\geometry_builder.h"
#include "NachosEngine\geometry.h"
#include "gpumanager.h"



namespace NachosEngine {

	GPU_Manager gpu_manager;

	Camera::Camera() {

		quad.allocT<Geometry>();

		GeometryBuilder tmp_gb;

		tmp_gb.Begin(4);

		float positions[12] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f
		};
		float uvs[8] = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
		};
		unsigned int elements[] = {
			0, 1, 2,
			2, 3, 0
		};

		tmp_gb.Add(kAttributeName_Position, kAttributeType_Float3, positions);
		tmp_gb.Add(kAttributeName_UV, kAttributeType_Float2, uvs);

		tmp_gb.End(quad.get(), elements, 6);

		drawable_quad_.allocT<Drawable>();
		drawable_quad_->set_geometry(quad.get());

		color_texture_ = Texture::Instance().GenerateEmptyTexture(Texture::Format::kFormat_RGB, 800, 600);
		depth_texture_ = Texture::Instance().GenerateEmptyTexture(Texture::Format::kFormat_DepthComponent, 800, 600);
		quad_mat.allocT<NachosEngine::Material>();
		quad_mat->LoadShader("Shaders/vertex_shader_render_to_texture.txt", "Shaders/fragment_shader_render_to_texture.txt");
		quad_mats.allocT<NachosEngine::MaterialSettings>();
		quad_mats->set_material(quad_mat.get());

		quad_mats->AddTexture(color_texture_);
		quad_mats->AddSamplerName("tex");
		drawable_quad_->set_material(quad_mat.get());
		drawable_quad_->set_materialsettings(quad_mats.get());

		light_color_texture_ = Texture::Instance().GenerateEmptyTexture(Texture::Format::kFormat_RGB, 800, 600);
		light_depth_texture_ = Texture::Instance().GenerateEmptyTexture(Texture::Format::kFormat_DepthComponent, 800, 600);


		glow_color_texture_ = Texture::Instance().GenerateEmptyTexture(Texture::Format::kFormat_RGB, 128, 128);
		glow_depth_texture_ = Texture::Instance().GenerateEmptyTexture(Texture::Format::kFormat_DepthComponent, 128, 128);
		glow_buffer_.viewport(128, 128);

		glow2_color_texture_ = Texture::Instance().GenerateEmptyTexture(Texture::Format::kFormat_RGB, 128, 128);
		glow2_depth_texture_ = Texture::Instance().GenerateEmptyTexture(Texture::Format::kFormat_DepthComponent, 128, 128);
		glow2_buffer_.viewport(128, 128);


		glow3_color_texture_ = Texture::Instance().GenerateEmptyTexture(Texture::Format::kFormat_RGB, 128, 128);
		glow3_depth_texture_ = Texture::Instance().GenerateEmptyTexture(Texture::Format::kFormat_DepthComponent, 128, 128);
		glow3_buffer_.viewport(128, 128);


		//Lightmap Material
		const char lightmap_vertex_shader[] = "Shaders/lightmap_vertex_shader.txt";
		const char lightmap_fragment_shader[] = "Shaders/lightmap_fragment_shader.txt";
		lightmap_material_.allocT<NachosEngine::Material>();
		lightmap_material_->LoadShader(lightmap_vertex_shader, lightmap_fragment_shader);
		lightmap_mats_.allocT<NachosEngine::MaterialSettings>();
		lightmap_mats_->set_material(lightmap_material_.get());

		//VBlur Material
		const char VBlur_vertex_shader[] = "Shaders/glowV_vertex_shader.txt";
		const char VBlur_fragment_shader[] = "Shaders/glow_fragment_shader.txt";
		Vblur_material_.allocT<NachosEngine::Material>();
		Vblur_material_->LoadShader(VBlur_vertex_shader, VBlur_fragment_shader);
		Vblur_mats_.allocT<NachosEngine::MaterialSettings>();
		Vblur_mats_->set_material(Vblur_material_.get());
		Vblur_mats_->AddSamplerName("glow_texture");
		Vblur_mats_->AddTexture(glow_color_texture_);


		//Hblur Material
		const char HBlur_vertex_shader[] = "Shaders/glowH_vertex_shader.txt";
		const char HBlur_fragment_shader[] = "Shaders/glow_fragment_shader.txt";
		Hblur_material_.allocT<NachosEngine::Material>();
		Hblur_material_->LoadShader(HBlur_vertex_shader, HBlur_fragment_shader);
		Hblur_mats_.allocT<NachosEngine::MaterialSettings>();
		Hblur_mats_->set_material(Hblur_material_.get());
		Hblur_mats_->AddSamplerName("glow_texture");
		Hblur_mats_->AddTexture(glow2_color_texture_);

		//Glowing objects with texture
		const char texture_vertex_shader[] = "Shaders/texture_vertex_shader.txt";
		const char texture_fragment_shader[] = "Shaders/texture_fragment_shader.txt";
		texture_mat_.allocT<NachosEngine::Material>();
		texture_mat_->LoadShader(texture_vertex_shader, texture_fragment_shader);
		texture_mats_.allocT<NachosEngine::MaterialSettings>();
		texture_mats_->set_material(texture_mat_.get());
		texture_mats_->AddSamplerName("tex");
		texture_mats_->AddTexture(0);

		//Glowing objects with texture
		const char glow_postprocess_vertex_shader[] = "Shaders/glow_render_to_texture_vertex.txt";
		const char glow_postprocess_fragment_shader[] = "Shaders/glow_render_to_texture_fragment.txt";
		glow_postprocess_mat_.allocT<NachosEngine::Material>();
		glow_postprocess_mat_->LoadShader(glow_postprocess_vertex_shader, glow_postprocess_fragment_shader);
		glow_postprocess_mats_.allocT<NachosEngine::MaterialSettings>();
		glow_postprocess_mats_->set_material(texture_mat_.get());
		glow_postprocess_mats_->AddSamplerName("tex");
		glow_postprocess_mats_->AddSamplerName("glow_tex");
		glow_postprocess_mats_->AddTexture(color_texture_);
		glow_postprocess_mats_->AddTexture(glow3_color_texture_);
	}

	Camera::~Camera() {

	}

	//Projection
	void Camera::set_fovy(const float fovy) {
		fovy_ = fovy;
	}
	void Camera::set_aspect_ratio(const float aspect_ratio) {
		aspect_ratio_ = aspect_ratio;
	}
	void Camera::set_znear(const float znear) {
		znear_ = znear;
	}
	void Camera::set_zfar(const float zfar) {
		zfar_ = zfar;
	}

	float* Camera::projection() {
		return glm::value_ptr(projection_);
	}

	//View
	void Camera::set_position(const float x, const float y, const float z) {
		position_ = glm::vec3(x, y, z);
	}
	void Camera::set_direction(const float x, const float y, const float z) {
		direction_ = glm::vec3(x, y, z);
	}
	void Camera::set_up_direction(const float x, const float y, const float z) {
		up_direction_ = glm::vec3(x, y, z);
	}

	void Camera::Move(const float x, const float y, const float z) {
		position_ += glm::vec3(x, y, z);
		direction_ += glm::vec3(x, y, z);
	}

	void Camera::Rotate(const float amount) {
		//direction_ = glm::rotate(direction_,amount,glm::vec3(0.0f,1.0f,0.0f));
		view_ = glm::rotate(view_, amount, glm::vec3(0.0f, 1.0f, 0.0f));
		printf("%f\n", amount);
	}

	void Camera::Update() {
		projection_ = glm::perspective(fovy_, aspect_ratio_, znear_, zfar_);
		view_ = glm::lookAt(position_, direction_, up_direction_);

	}

	void Camera::set_root(ref_ptr<Node> root) {
		root_ = root;
	}

	float* Camera::view() {
		return glm::value_ptr(view_);
	}

	float* Camera::light_view() {
		return glm::value_ptr(light_view_);
	}

	unsigned int Camera::light_depth_texture() {
		return light_depth_texture_;
	}

	unsigned int Camera::blurred_texture() {
		return glow3_color_texture_;
	}

	void Camera::Draw() {

		//Empty Render bin
		if (render_bin_.size() > 0) {
			render_bin_.erase(render_bin_.begin(), render_bin_.begin() + render_bin_.size());
		}

		//Empty used programs list
		if (used_programs_.size() > 0) {
			used_programs_.erase(used_programs_.begin(), used_programs_.begin() + used_programs_.size());
		}

		//free the vector allocated memory
		render_bin_.shrink_to_fit();
		used_programs_.shrink_to_fit();

		assert(root_ == NULL || "Must set root node for camera before drawing");

		//Store used programs
		for (unsigned int i = 0; i < root_->childs().size(); ++i) {
			bool found = false;
			Drawable *drawable = dynamic_cast<Drawable*>(root_->childs()[i].get());
			if (drawable != NULL) {
				for (unsigned int j = 0; j < used_programs_.size(); ++j) {
					if (used_programs_[j] == drawable->material()->internal_material->program_) {
						found = true;
					}
				}
				if (!found) {
					used_programs_.push_back(drawable->material()->internal_material->program_);
				}
			}
		}

		//push to render bin, ordered by program
		render_bin_.resize(root_->childs().size());
		for (unsigned int i = 0; i < root_->childs().size(); ++i) {
			Drawable *drawable = dynamic_cast<Drawable*>(root_->childs()[i].get());
			for (unsigned int j = 0; j < used_programs_.size(); ++j) {
				if (drawable->material()->internal_material->program_ == used_programs_[j]) {
					//render_bin_.push_back(root_->childs()[i]);
					render_bin_[i].geometry = drawable->geometry();
					render_bin_[i].material = drawable->material();
					render_bin_[i].material_settings = drawable->materialsettings();
					render_bin_[i].model = drawable->model_as_mat();
					render_bin_[i].view = view_;
					render_bin_[i].projection = projection_;
				}
			}
		}

		//Draw
		for (unsigned int i = 0; i < render_bin_.size(); ++i) {
			render_bin_[i].material_settings->UniformValue("model_matrix", glm::value_ptr(render_bin_[i].model));
			render_bin_[i].material_settings->UniformValue("projection_matrix", glm::value_ptr(projection_));
			render_bin_[i].material_settings->UniformValue("view_matrix", glm::value_ptr(view_));
			render_bin_[i].material_settings->UniformValue("light_view", glm::value_ptr(light_view_));
			render_bin_[i].material_settings->UniformValue("light_projection", glm::value_ptr(light_projection_));
			gpu_manager.Draw(render_bin_[i].geometry, render_bin_[i].material, render_bin_[i].material_settings, kTriangles);
		}

	}

	void Camera::DrawLightMap() {

		//Empty Render bin
		if (render_bin_by_mat_.size() > 0) {
			render_bin_by_mat_.erase(render_bin_by_mat_.begin(), render_bin_by_mat_.begin() + render_bin_by_mat_.size());
		}

		//Empty used programs list
		if (used_programs_.size() > 0) {
			used_programs_.erase(used_programs_.begin(), used_programs_.begin() + used_programs_.size());
		}

		//free the vector allocated memory
		render_bin_by_mat_.shrink_to_fit();
		used_programs_.shrink_to_fit();

		assert(root_ == NULL || "Must set root node for camera before drawing");

		//Store used programs
		for (unsigned int i = 0; i < root_->childs().size(); ++i) {
			bool found = false;
			Drawable *drawable = dynamic_cast<Drawable*>(root_->childs()[i].get());
			if (drawable != NULL) {
				for (unsigned int j = 0; j < used_programs_.size(); ++j) {
					if (used_programs_[j] == drawable->material()->internal_material->program_) {
						found = true;
					}
				}
				if (!found) {
					used_programs_.push_back(drawable->material()->internal_material->program_);
				}
			}
		}

		//push to render bin, ordered by program
		for (unsigned int i = 0; i < root_->childs().size(); ++i) {
			Drawable *drawable = dynamic_cast<Drawable*>(root_->childs()[i].get());
			for (unsigned int j = 0; j < used_programs_.size(); ++j) {
				if (drawable->material()->internal_material->program_ == used_programs_[j]) {
					render_bin_by_mat_.push_back(root_->childs()[i]);
				}
			}
		}

		//Draw
		for (unsigned int i = 0; i < render_bin_by_mat_.size(); ++i) {
			Drawable *drawable = dynamic_cast<Drawable*>(render_bin_by_mat_[i].get());
			lightmap_mats_->UniformValue("zmodel_matrix", drawable->model());
			lightmap_mats_->UniformValue("view_matrix", glm::value_ptr(light_view_));
			lightmap_mats_->UniformValue("projection_matrix", glm::value_ptr(light_projection_));
			gpu_manager.Draw(drawable->geometry(), lightmap_material_.get(), lightmap_mats_.get(), kTriangles);
		}

	}

	void Camera::DrawLightMap(glm::mat4 light_view, glm::mat4 light_projection) {

		//Empty Render bin
		if (render_bin_.size() > 0) {
			render_bin_.erase(render_bin_.begin(), render_bin_.begin() + render_bin_.size());
		}

		//Empty used programs list
		if (used_programs_.size() > 0) {
			used_programs_.erase(used_programs_.begin(), used_programs_.begin() + used_programs_.size());
		}

		//free the vector allocated memory
		render_bin_.shrink_to_fit();
		used_programs_.shrink_to_fit();

		assert(root_ == NULL || "Must set root node for camera before drawing");

		//Store used programs
		for (unsigned int i = 0; i < root_->childs().size(); ++i) {
			bool found = false;
			Drawable *drawable = dynamic_cast<Drawable*>(root_->childs()[i].get());
			if (drawable != NULL) {
				for (unsigned int j = 0; j < used_programs_.size(); ++j) {
					if (used_programs_[j] == drawable->material()->internal_material->program_) {
						found = true;
					}
				}
				if (!found) {
					used_programs_.push_back(drawable->material()->internal_material->program_);
				}
			}
		}

		//push to render bin, ordered by program
		render_bin_.resize(root_->childs().size());
		for (unsigned int i = 0; i < root_->childs().size(); ++i) {
			Drawable *drawable = dynamic_cast<Drawable*>(root_->childs()[i].get());
			for (unsigned int j = 0; j < used_programs_.size(); ++j) {
				if (drawable->material()->internal_material->program_ == used_programs_[j]) {
					//render_bin_.push_back(root_->childs()[i]);
					render_bin_[i].geometry = drawable->geometry();
					render_bin_[i].material = drawable->material();
					render_bin_[i].material_settings = drawable->materialsettings();
					render_bin_[i].model = drawable->model_as_mat();
					render_bin_[i].view = view_;
					render_bin_[i].projection = projection_;
				}
			}
		}

		//Draw
		for (unsigned int i = 0; i < render_bin_.size(); ++i) {
			lightmap_mats_->UniformValue("zmodel_matrix", glm::value_ptr(render_bin_[i].model));
			lightmap_mats_->UniformValue("view_matrix", glm::value_ptr(light_view));
			lightmap_mats_->UniformValue("projection_matrix", glm::value_ptr(light_projection));
			gpu_manager.Draw(render_bin_[i].geometry, lightmap_material_.get(), lightmap_mats_.get(), kTriangles);
		}

	}

	void Camera::DrawInWindow() {
		glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//empty the vectors
		//Bind frame buffer

		//calculate depth texture
		light_buffer_.Activate();

		light_buffer_.AttachTexture(FrameBuffer::Attachment::kAttachment_Color, light_color_texture_);
		light_buffer_.AttachTexture(FrameBuffer::Attachment::kAttachment_Depth, light_depth_texture_);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "DrawInWindow - Camera class, light_buffer_ error");

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DrawLightMap(light_view_, light_projection_);

		light_buffer_.Deactivate();


		//Texture with only Glowing objects
		glow_buffer_.Activate();

		glow_buffer_.AttachTexture(FrameBuffer::Attachment::kAttachment_Color, glow_color_texture_);
		glow_buffer_.AttachTexture(FrameBuffer::Attachment::kAttachment_Depth, glow_depth_texture_);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "DrawInWindow - Camera class, glow_buffer_ error");

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DrawGlowObjects();

		glow_buffer_.Deactivate();

		//Vertical Blur
		glow2_buffer_.Activate();

		glow2_buffer_.AttachTexture(FrameBuffer::Attachment::kAttachment_Color, glow2_color_texture_);
		glow2_buffer_.AttachTexture(FrameBuffer::Attachment::kAttachment_Depth, glow2_depth_texture_);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "DrawInWindow - Camera class, glow2_buffer_ error");

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gpu_manager.Draw(drawable_quad_->geometry(), Vblur_material_.get(), Vblur_mats_.get(), kTriangles);

		glow2_buffer_.Deactivate();

		//Horizontal Blur
		glow3_buffer_.Activate();

		glow3_buffer_.AttachTexture(FrameBuffer::Attachment::kAttachment_Color, glow3_color_texture_);
		glow3_buffer_.AttachTexture(FrameBuffer::Attachment::kAttachment_Depth, glow3_depth_texture_);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "DrawInWindow - Camera class, glow2_buffer_ error");

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gpu_manager.Draw(drawable_quad_->geometry(), Hblur_material_.get(), Hblur_mats_.get(), kTriangles);

		glow3_buffer_.Deactivate();

		//Draw();
		//Draw Scene to Texture (color_texture_);
		DrawInTexture();

		gpu_manager.Draw(drawable_quad_->geometry(), glow_postprocess_mat_.get(), glow_postprocess_mats_.get(), kTriangles);

	}

	void Camera::DrawInTexture(int level) {

		//Bind framebuffer
		frame_buffer_.Activate();

		frame_buffer_.AttachTexture(FrameBuffer::Attachment::kAttachment_Color, color_texture_);
		frame_buffer_.AttachTexture(FrameBuffer::Attachment::kAttachment_Depth, depth_texture_);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "DrawInTexture - Camera class, Framebuffer error");

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Draw();

		//Bind Window
		frame_buffer_.Deactivate();

		//Draw
		//gpu_manager.Draw(drawable_quad_->geometry(), drawable_quad_->material(), drawable_quad_->materialsettings(), kTriangles);


	}


	//MAL, debe actualizarse la textura cada frame, aqui solo se actualiza el light_view_
	void Camera::CalculateLightDepth() {

		//Set light at camera position
		//light_view_ = view_ * glm::translate(glm::vec3(-0.0f,12.0f,0.0f));
    	light_view_ = view_;
		light_projection_ = projection_;

	}

	void Camera::DrawGlowObjects() {
		//Empty Render bin
		if (glow_render_bin_.size() > 0) {
			glow_render_bin_.erase(glow_render_bin_.begin(), glow_render_bin_.begin() + glow_render_bin_.size());
		}

		//Empty used programs list
		if (used_programs_.size() > 0) {
			used_programs_.erase(used_programs_.begin(), used_programs_.begin() + used_programs_.size());
		}

		//free the vector allocated memory
		glow_render_bin_.shrink_to_fit();
		used_programs_.shrink_to_fit();

		assert(root_ == NULL || "Must set root node for camera before drawing");

		//Store used programs
		for (unsigned int i = 0; i < root_->childs().size(); ++i) {
			bool found = false;
			Drawable *drawable = dynamic_cast<Drawable*>(root_->childs()[i].get());
			if (drawable != NULL) {
				for (unsigned int j = 0; j < used_programs_.size(); ++j) {
					if (used_programs_[j] == drawable->material()->internal_material->program_) {
						found = true;
					}
				}
				if (!found) {
					used_programs_.push_back(drawable->material()->internal_material->program_);
				}
			}
		}

		//Number of glow objects
		unsigned int glow_objects = 0;
		for (unsigned int i = 0; i < root_->childs().size(); ++i) {
			Drawable *drawable = dynamic_cast<Drawable*>(root_->childs()[i].get());
			if ((drawable->postprocess_ & 0x01) == 0x01) {
				glow_objects++;
			}
		}

		//push to render bin, ordered by program
		unsigned int current_position = 0;
		glow_render_bin_.resize(glow_objects);
		for (unsigned int i = 0; i < root_->childs().size(); ++i) {
			Drawable *drawable = dynamic_cast<Drawable*>(root_->childs()[i].get());
			for (unsigned int j = 0; j < used_programs_.size(); ++j) {
				if (drawable->material()->internal_material->program_ == used_programs_[j] && (drawable->postprocess_ & 0x01) == 0x01) {
					glow_render_bin_[current_position].geometry = drawable->geometry();
					glow_render_bin_[current_position].material = drawable->material();
					glow_render_bin_[current_position].material_settings = drawable->materialsettings();
					glow_render_bin_[current_position].model = drawable->model_as_mat();
					glow_render_bin_[current_position].view = view_;
					glow_render_bin_[current_position].projection = projection_;
					current_position++;
				}
			}
		}

		//Draw
		for (unsigned int i = 0; i < glow_render_bin_.size(); ++i) {
			texture_mats_->UniformValue("model_matrix", glm::value_ptr(glow_render_bin_[i].model));
			texture_mats_->UniformValue("projection_matrix", glm::value_ptr(projection_));
			texture_mats_->UniformValue("view_matrix", glm::value_ptr(view_));
			unsigned int tmp = glow_render_bin_[i].material_settings->internal_material_settings->GetTexture();
			texture_mats_->internal_material_settings->OverwriteTexture0(glow_render_bin_[i].material_settings->internal_material_settings->GetTexture());
			unsigned int tmp2 = texture_mats_->internal_material_settings->GetTexture();
			gpu_manager.Draw(glow_render_bin_[i].geometry, texture_mat_.get(), texture_mats_.get(), kTriangles);
			//gpu_manager.Draw(glow_render_bin_[i].geometry, glow_render_bin_[i].material, glow_render_bin_[i].material_settings, kTriangles);
		}
	}

} //end namespace NachosEngine

