#pragma once
#include "Geometry.h"
#include <glm/glm.hpp>

class Plane : public Geometry {
public:
	Plane() = default;
	Plane(const Transform& transform, std::shared_ptr<Material> material) :
		Geometry{ transform, material }
	{}

	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDist, float maxDist) override;
	static bool Raycast(const ray_t& ray, const glm::vec3& point, const glm::vec3& normal, float minDist, float maxDist, float& t);
private:
};