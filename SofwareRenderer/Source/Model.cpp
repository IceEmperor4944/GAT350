#include "Model.h"
#include "Framebuffer.h"
#include "Shader.h"
#include <iostream>
#include <sstream>
#include <fstream>

bool Model::Load(const std::string& filename) {
	// open file using ifstream (input file stream)
	std::ifstream stream(filename);
	// check if stream is_open
	if (!stream.is_open()) {
		// TODO: report error to cerr and return false
		std::cerr << "Stream failed to open." << std::endl;
		return false;
	}

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::string line;
	while (std::getline(stream, line)) {
		// read in vertex positions
		if (line.substr(0, 2) == "v ") {
			// read position of vertex
			std::istringstream sstream{ line.substr(2) };
			glm::vec3 position;
			sstream >> position.x;
			sstream >> position.y;
			sstream >> position.z;

			vertices.push_back(position);
		}
		// read in vertex normals
		else if (line.substr(0, 3) == "vn ") {
			// read position of vertex
			std::istringstream sstream{ line.substr(3) };
			glm::vec3 normal;
			sstream >> normal.x;
			sstream >> normal.y;
			sstream >> normal.z;

			normals.push_back(normal);
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
				if (index[0] && index[1]) {
					// add vertex to model vertices
					vertex_t vertex;
					vertex.position = vertices[index[0] - 1];
					vertex.normal = (index[2]) ? normals[index[2] - 1] : glm::vec3{ 1 };
					m_vb.push_back(vertex);
				}
			}
		}
	}

	stream.close();

	return true;
}

void Model::Draw() {
	Shader::Draw(m_vb);

	/*for (int i = 0; i < m_vb.size(); i += 3) {
		vertex_t p1 = model * glm::vec4{ m_vb[i + 0], 1 };
		vertex_t p2 = model * glm::vec4{ m_vb[i + 1], 1 };
		vertex_t p3 = model * glm::vec4{ m_vb[i + 2], 1 };

		p1 = camera.ModelToView(p1);
		p2 = camera.ModelToView(p2);
		p3 = camera.ModelToView(p3);

		glm::ivec2 s1 = camera.ViewToScreen(p1);
		glm::ivec2 s2 = camera.ViewToScreen(p2);
		glm::ivec2 s3 = camera.ViewToScreen(p3);

		if (s1.x == -1 || s1.y == -1 || s2.x == -1 || s2.y == -1 || s3.x == -1 || s3.y == -1) {
			continue;
		}

		buffer.DrawTriangle(s1.x, s1.y, s2.x, s2.y, s3.x, s3.y, ColorConvert(m_color));
	}*/
}