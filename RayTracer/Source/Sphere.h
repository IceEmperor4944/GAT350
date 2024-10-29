#pragma once
#include "Geometry.h"
#include <glm/glm.hpp>

class Sphere : public Geometry {
public:
	Sphere() = default;
	Sphere(const glm::vec3& center, float radius, std::shared_ptr<Material> material) :
		Geometry{ material },
		m_center{ center },
		m_radius{ radius }
	{}

	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDist, float maxDist) override;
private:
	glm::vec3 m_center;
	float m_radius = 0;
};