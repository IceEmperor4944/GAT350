#pragma once
#include "Model.h"
#include "Transform.h"
#include <memory>

class Actor {
public:
	Actor(const Transform& transform, std::shared_ptr<Model> model) : m_transform{ transform }, m_model{ model } {}

	Transform& GetTransform() { return m_transform; }
	const Transform& GetTransform() const { return m_transform; }
	
	void Draw(class Framebuffer& buffer, const class Camera& camera);
	void SetColor(const color_t& color) { m_color = color; }
private:
	Transform m_transform;
	std::shared_ptr<Model> m_model;
	color_t m_color{ 255, 255, 255, 255 };
};