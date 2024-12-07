#include "FragmentShader.h"
#include "Shader.h"

color4_t FragmentShader::Process(const fragment_input_t& fragment) {
	//transform light to view
	glm::vec3 lightPos = Shader::uniforms.view * glm::vec4{ Shader::uniforms.light.position, 1 };
	glm::vec3 lightDirec = Shader::uniforms.view * glm::vec4{ -Shader::uniforms.light.direction, 0 };

	glm::vec3 lightDir = glm::normalize(lightPos - fragment.position); // normalize light direction
	//glm::vec3 lightDir = glm::normalize(lightDirec);

	//diffuse
	float intensity = std::max(0.0f, glm::dot(lightDir, fragment.normal));
	color3_t diffuse = Shader::uniforms.light.color * intensity;

	//specular
	color3_t specular = color3_t{ 0 }; // Initialize specular color to black (no contribution)
	if (intensity > 0) {
		glm::vec3 reflection = glm::reflect(-lightDir, fragment.normal); // Compute reflection vector
		glm::vec3 vDir = glm::normalize(-fragment.position); // Calculate view direction
		intensity = glm::max(glm::dot(reflection, vDir), 0.0f); // Compute intensity of reflection
		intensity = pow(intensity, Shader::uniforms.material.shininess); // Apply shininess factor
		specular = Shader::uniforms.material.specular * intensity; // Scale specular by intensity and material property
	}

	color3_t color = ((Shader::uniforms.ambient + diffuse) * Shader::uniforms.material.albedo) + specular;
	return { color, 1 };
}