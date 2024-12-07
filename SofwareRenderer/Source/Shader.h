#pragma once
#include "VertexShader.h"
#include "FragmentShader.h"

class Shader {
public:
	struct uniforms_t
	{
		// transformations (mvp)
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		// lights
		light_t light;
		color3_t ambient;
		// materials
		material_t material;
	};
public:
	enum eFrontFace {
		CW,
		CCW
	};

	enum eCullMode {
		FRONT,
		BACK,
		NONE
	};
public:
	static void Draw(const vertexbuffer_t& vb);
	static bool ToScreen(const vertex_output_t& vertex, glm::vec2& screen);
public:
	static class Framebuffer* framebuffer;
	static eFrontFace front_face;
	static eCullMode cull_mode;

	static uniforms_t uniforms;
};