#include "VertexShader.h"
#include "Framebuffer.h"
#include "Shader.h"

//#define GOURAUD	

void VertexShader::Process(const vertex_t& ivertex, vertex_output_t& overtex) {
	glm::mat4 mvp = Shader::uniforms.projection * Shader::uniforms.view * Shader::uniforms.model;
	overtex.position = mvp * glm::vec4{ ivertex.position, 1 };
	glm::mat4 mv = Shader::uniforms.view * Shader::uniforms.model;
	overtex.normal = glm::normalize(glm::mat3{ mv } * ivertex.normal);
	overtex.vPos = mv * glm::vec4{ ivertex.position, 1 };

#ifdef GOURAUD
	glm::vec3 lightPos = Shader::uniforms.view * glm::vec4{ Shader::uniforms.light.position, 1 };
	glm::vec3 lightDirec = Shader::uniforms.view * glm::vec4{ -Shader::uniforms.light.direction, 0 };

	glm::vec3 vPos = mv * glm::vec4{ ivertex.position, 1 };
	glm::vec3 lightDir = glm::normalize(lightPos - vPos); // normalize light direction
	//glm::vec3 lightDir = glm::normalize(lightDirec);

	float intensity = std::max(0.0f, glm::dot(lightDir, overtex.normal));
	color3_t diffuse = Shader::uniforms.light.color * intensity;
	color3_t specular = color3_t{ 0 }; // Initialize specular color to black (no contribution)

	if (intensity > 0) {
		glm::vec3 reflection = glm::reflect(-lightDirec, overtex.normal); // Compute reflection vector
		glm::vec3 vDir = glm::normalize(-vPos); // Calculate view direction
		intensity = glm::max(glm::dot(reflection, vDir), 0.0f); // Compute intensity of reflection
		intensity = glm::pow(intensity, Shader::uniforms.material.shininess); // Apply shininess factor
		specular = Shader::uniforms.material.specular * intensity; // Scale specular by intensity and material property
	}

	overtex.color = ((Shader::uniforms.ambient + diffuse) * Shader::uniforms.material.albedo) + specular;
#endif // 0
}