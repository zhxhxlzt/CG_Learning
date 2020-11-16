#pragma once

#include <vector>


#include "RayCast.h"
#include "Shader.h"
#include "glad/glad.h"

class CAABoundingBox : public CRayCastable
{
public:
	void Build(const std::vector<glm::vec3>& vertices);
	bool RayCast(const CRay& ray, CRayCastInfo& info) override;

	void Draw(CShader& shader);
	
private:
	glm::vec3 m_minPos;
	glm::vec3 m_maxPos;
	bool m_inited = false;
	GLuint vao, vbo, ebo;
	std::vector<glm::vec3> m_vertices;
	std::vector<unsigned int> m_indices;
};