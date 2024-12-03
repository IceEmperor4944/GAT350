#include "Actor.h"
#include "Shader.h"

void Actor::Draw() {
	VertexShader::uniforms.model = m_transform.GetMatrix();
	VertexShader::uniforms.material = *m_material;
	m_model->Draw();
}