#pragma once
#include "Material.h"
#include "Ray.h"
#include "Transform.h"
#include <memory>

class Geometry { ///////this is Maple's Scene Object
public:
	Geometry() = default;
	Geometry(const Transform& transform, std::shared_ptr<Material> material) :
		m_transform{ transform },
		m_material{ material } {}
	Geometry(std::shared_ptr<Material> material) : m_material{ material } {}

	virtual void Update() {}
	virtual bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDist, float maxDist) = 0;

	std::weak_ptr<Material> GetMaterial() { return m_material; }
	Transform& GetTransform() { return m_transform; }
	const Transform& GetTransform() const { return m_transform; }
protected:
	std::shared_ptr<Material> m_material;
	Transform m_transform;
};