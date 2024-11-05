#pragma once
#include "Geometry.h"
#include <glm/glm.hpp>

class Sphere : public Geometry {
public:
	Sphere() = default;
	Sphere(const Transform& transform, float radius, std::shared_ptr<Material> material) :
		Geometry{ transform, material },
		m_radius{ radius }
	{}

	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDist, float maxDist) override;
	static bool Raycast(const ray_t& ray, const glm::vec3& center, float radius, float minDist, float maxDist, float& t);
private:
	float m_radius = 0;
};