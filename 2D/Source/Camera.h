#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include <glm/glm.hpp>

class Camera {
public:
	Camera(int width, int height) : screenWidth{ width }, screenHeight{ height } {}

	void SetView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up = { 0, 1, 0 });
	void SetProjection(float fov, float aspect, float near, float far);

	glm::vec3 ModelToView(const glm::vec3& pos) const;
	glm::vec4 ViewToProjection(const glm::vec3& pos) const;
	glm::ivec2 ViewToScreen(const glm::vec3& pos) const;

	const glm::mat4& GetView() { return view; }
	const glm::mat4& GetProjection() { return projection; }
private:
	glm::mat4 view{ 0 };
	glm::mat4 projection{ 0 };

	int screenWidth = 0;
	int screenHeight = 0;
};