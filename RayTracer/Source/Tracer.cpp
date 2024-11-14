#include "Tracer.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Color.h"
#include "MathUtils.h"
#include "Scene.h"
#include <glm/glm.hpp>
#include <iostream>

color3_t Tracer::Trace(Scene& scene, const ray_t& ray, float minDist, float maxDist, int depth) {
	//std::cout << "depth: " << depth << std::endl;
	if (depth == 0) return color3_t{ 0 };

	raycastHit_t raycastHit;
	float closestDistance = maxDist;
	bool isHit = false;
	
	for (auto& object : scene.m_objects) {
		if (object->Hit(ray, raycastHit, minDist, closestDistance)) {
			isHit = true;
			closestDistance = raycastHit.distance;
		}
	}

	if (isHit) {
		color3_t attenuation;
		ray_t scatter;
		if (raycastHit.material.lock()->Scatter(ray, raycastHit, attenuation, scatter)) {
			return attenuation * Trace(scene, scatter, minDist, maxDist, depth - 1);
		}
		else {
			return raycastHit.material.lock()->GetEmissive();
		}
	}

	//sky
	glm::vec3 direction = glm::normalize(ray.direction);
	float t = (direction.y + 1) * 0.5f;
	color3_t color = Lerp(scene.m_skyBottom, scene.m_skyTop, t);

	return color;
}