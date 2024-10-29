#pragma once
#include "Geometry.h"
#include <glm/glm.hpp>

class Plane : public Geometry {
public:
	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDist, float maxDist) override;
private:
	glm::vec3 m_center;
};