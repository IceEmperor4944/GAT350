#include "VertexShader.h"

VertexShader::uniforms_t VertexShader::uniforms = {
	glm::mat4{1}, //model
	glm::mat4{1}, //view
	glm::mat4{1}, //projection
	color3_t{1}
};

void VertexShader::Process(const vertex_t& ivertex, vertex_output_t& overtex) {
	glm::mat4 mvp = uniforms.projection * uniforms.view * uniforms.model;
	overtex.position = mvp * glm::vec4{ ivertex.position, 1 };
	glm::mat4 mv = uniforms.view * uniforms.model;
	overtex.normal = glm::normalize(glm::mat3{ mv } * ivertex.normal);

	glm::vec3 lightPos = uniforms.view * glm::vec4{ uniforms.light.position, 1 };
	glm::vec3 lightDirec = uniforms.view * glm::vec4{ -uniforms.light.direction, 0 };

	glm::vec3 vPos = mv * glm::vec4{ ivertex.position, 1};
	glm::vec3 lightDir = glm::normalize(lightPos - vPos); // normalize light direction
	//glm::vec3 lightDir = glm::normalize(lightDirec);

	float intensity = std::max(0.0f, glm::dot(lightDir, overtex.normal));
	color3_t diffuse = uniforms.light.color * intensity;
	color3_t specular = color3_t{ 0 }; // Initialize specular color to black (no contribution)

	if (intensity > 0) {
		glm::vec3 reflection = glm::reflect(-lightDirec, overtex.normal); // Compute reflection vector
		glm::vec3 vDir = glm::normalize(-vPos); // Calculate view direction
		intensity = glm::max(glm::dot(reflection, vDir), 0.0f); // Compute intensity of reflection
		intensity = glm::pow(intensity, uniforms.material.shininess); // Apply shininess factor
		specular = uniforms.material.specular * intensity; // Scale specular by intensity and material property
	}

	overtex.color = ((uniforms.ambient + diffuse) * uniforms.material.albedo) + specular;
}