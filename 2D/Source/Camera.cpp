#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::SetView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) {
	view = glm::lookAt(eye, target, up);
}

void Camera::SetProjection(float fov, float aspect, float near, float far) {
	projection = glm::perspective(fov, aspect, near, far);
}

glm::vec3 Camera::ModelToView(const glm::vec3& pos) const {
	return view * glm::vec4{ pos, 1 };
}

glm::vec4 Camera::ViewToProjection(const glm::vec3& pos) const {
	return projection * glm::vec4{ pos, 1 };
}

glm::ivec2 Camera::ViewToScreen(const glm::vec3& pos) const {
	glm::vec4 clip = ViewToProjection(pos);
	glm::vec3 ndc = clip / clip.w;
	float x = (ndc.x + 1) * (screenWidth * 0.5f);
	float y = (1 - ndc.y + 1) * (screenHeight * 0.5f);
	
	return glm::ivec2(x, y);
}