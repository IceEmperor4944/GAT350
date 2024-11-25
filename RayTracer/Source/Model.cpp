#include "Model.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Triangle.h"
#include "Sphere.h"
#include <iostream>
#include <sstream>
#include <fstream>

void Model::Update() {
	for (size_t i = 0; i < m_localVertices.size(); i++) {
		m_vb[i] = m_transform * glm::vec4{ m_localVertices[i], 1 };
	}

	// get center point of transformed vertices
	m_center = glm::vec3{ 0 };
	for (auto& vertex : m_vb)
	{
		m_center += vertex;
	}
	m_center /= (float)m_vb.size();

	// get radius of transformed vertices
	m_radius = 0;
	for (auto& vertex : m_vb)
	{
		float radius = glm::length(vertex - m_center);
		m_radius = glm::max(radius, m_radius);
	}
}

bool Model::Load(const std::string& filename) {
	// open file using ifstream (input file stream)
	std::ifstream stream(filename);
	// check if stream is_open
	if (!stream.is_open()) {
		// TODO: report error to cerr and return false
		std::cerr << "Stream failed to open." << std::endl;
		return false;
	}

	vertices_t vertices;
	std::string line;
	while (std::getline(stream, line)) {
		// read in vertex positions
		// https://cplusplus.com/reference/string/string/substr/
		if (line.substr(0, 2) == "v ") {
			// read position of vertex
			std::istringstream sstream{ line.substr(2) };
			glm::vec3 position;
			sstream >> position.x;
			sstream >> position.y;
			sstream >> position.z;

			vertices.push_back(position);
		}
		// read in faces (triangles)
		else if (line.substr(0, 2) == "f ") {
			// read face (triangle), index of vertex position in vertices array [1, 2, 3]
			std::istringstream sstream{ line.substr(2) };
			std::string str;
			while (std::getline(sstream, str, ' ')) {
				std::istringstream sstream(str);
				std::string indexString;

				// read each triangle index
				size_t i = 0;
				unsigned int index[3] = { 0, 0, 0 }; // position, normal, uv
				while (std::getline(sstream, indexString, '/')) {
					if (!indexString.empty()) {
						std::istringstream indexStream{ indexString };
						indexStream >> index[i];
					}
					i++;
				}

				// check if index is valid (not 0)
				if (index[0]) {
					// get vertex at index position
					glm::vec3 position = vertices[index[0] - 1];
					// add vertex to model vertices
					m_localVertices.push_back(position);
				}
			}
		}
	}

	m_vb.resize(m_localVertices.size());
	stream.close();
	return true;
}

bool Model::Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDist, float maxDist) {
	//sphere shit
	
	for (size_t i = 0; i < m_localVertices.size(); i += 3) {
		// check for bounding sphere raycast
		float t;
		if (!Sphere::Raycast(ray, m_center, m_radius, minDist, maxDist, t)) return false;

		//check cast ray with mesh triangles
		if (Triangle::Raycast(ray, m_vb[i], m_vb[i + 1], m_vb[i + 2], minDist, maxDist, t)) {
			raycastHit.distance = t;
			raycastHit.point = ray.at(t);

			// set edges of the triangle
			glm::vec3 edge1 = m_vb[i + 1] - m_vb[i];
			glm::vec3 edge2 = m_vb[i + 2] - m_vb[i];

			// set raycast hit
			raycastHit.normal = glm::normalize(glm::cross(edge1, edge2));
			raycastHit.material = GetMaterial();
			return true;
		}
		
		m_vb[i] = m_transform * glm::vec4{ m_localVertices[i], 1 };
	}
	return false;
}