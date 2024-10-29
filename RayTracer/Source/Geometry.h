#pragma once
#include "Material.h"
#include "Ray.h"
#include <memory>

class Geometry { ///////this is Maple's Scene Object
public:
	Geometry() = default;
	Geometry(std::shared_ptr<Material> material) : m_material{ material } {}

	virtual bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDist, float maxDist) = 0;

	std::weak_ptr<Material> GetMaterial() { return m_material; }
private:
	std::shared_ptr<Material> m_material;
};