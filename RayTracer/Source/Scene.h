#pragma once
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include <vector>
#include <memory>

class Scene {
public:
	Scene() = default;

	void Update();
	void Render(class Framebuffer& framebuffer, const class Camera& camera, int numSamples = 10, int depth = 5);
	void AddObject(std::unique_ptr<Geometry> object) { m_objects.push_back(std::move(object)); }

	void SetSky(const color3_t& bottom, const color3_t& top) {
		m_skyBottom = bottom;
		m_skyTop = top;
	}

	friend class Tracer;
private:
	std::vector<std::unique_ptr<Geometry>> m_objects;
	color3_t m_skyBottom{ 1 };
	color3_t m_skyTop{ 0.5f, 0.7f, 1.0f };
};