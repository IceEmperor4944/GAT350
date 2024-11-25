#include "Actor.h"
#include "Shader.h"

void Actor::Draw(Framebuffer& buffer, const Camera& camera) {
	VertexShader::uniforms.model = m_transform.GetMatrix();
	m_model->Draw(buffer, m_transform.GetMatrix(), camera);
}