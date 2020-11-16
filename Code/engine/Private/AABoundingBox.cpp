#include "AABoundingBox.h"
#include <algorithm>
#include <array>
#include <vector>
#include <glad/glad.h>


void CAABoundingBox::Build(const std::vector<glm::vec3>& vertices)
{
	using namespace std;
	using namespace glm;
	for (auto& v : vertices)
	{
		if (!m_inited)
		{
			m_minPos = m_maxPos = v;
			m_inited = true;
			continue;
		}
		if (v.x > m_maxPos.x)
		{
			m_maxPos.x = v.x;
		}
		else if (v.x < m_minPos.x)
		{
			m_minPos.x = v.x;
		}

		if (v.y > m_maxPos.y)
		{
			m_maxPos.y = v.y;
		}
		else if (v.y < m_minPos.y)
		{
			m_minPos.y = v.y;
		}

		if (v.z > m_maxPos.z)
		{
			m_maxPos.z = v.z;
		}
		else if (v.z < m_minPos.z)
		{
			m_minPos.z = v.z;
		}
	}

	{
		vector<vec3> &vertices = m_vertices;

		vertices.push_back(m_minPos);
		vertices.push_back({ m_minPos.x, m_minPos.y, m_maxPos.z });
		vertices.push_back({ m_minPos.x, m_maxPos.y, m_maxPos.z });
		vertices.push_back({ m_minPos.x, m_maxPos.y, m_minPos.z });
		
		vertices.push_back(m_maxPos);
		vertices.push_back({ m_maxPos.x, m_minPos.y, m_maxPos.z });
		vertices.push_back({ m_maxPos.x, m_minPos.y, m_minPos.z });
		vertices.push_back({ m_maxPos.x, m_maxPos.y, m_minPos.z });

		// vertices.push_back(m_maxPos);
		// vertices.push_back({ m_minPos.x, m_maxPos.y, m_minPos.z });
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		int size = sizeof(vec3) * vertices.size();
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		vector<unsigned int>& indices = m_indices;
		indices = {
			0, 1, 2,
			1, 2, 3,
			4, 5, 6,
			4, 6, 7,
			1, 5, 4,
			1, 4, 2,
			6, 0, 3,
			6, 3, 7,
			//2, 4, 7,
			//2, 7, 3,
			//1, 5, 6,
			//1, 6, 0
			//5, 6, 7,
			//5, 7, 8
			//2, 5, 8, 2, 8, 3, 5, 6, 7, 5, 7, 8, 6, 1, 4, 6, 4, 7, 1, 2, 5, 1, 5, 6, 4, 3, 8, 4, 8, 7 };
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), indices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), NULL);
		glEnableVertexAttribArray(0);
	}
	
}

bool CAABoundingBox::RayCast(const CRay& ray, CRayCastInfo& info)
{
	using namespace glm;

	// 为0时加下很小的非零项，避免除0错误
	vec3 direction = ray.direction + vec3(glm::equal(ray.direction, vec3(0))) * vec3(1e-7);	

	vec3 tMins = (m_minPos - ray.position) / direction;
	vec3 tMaxs = (m_maxPos - ray.position) / direction;

	vec3 tRealMins = glm::min(tMins, tMaxs);
	vec3 tRealMaxs = glm::max(tMins, tMaxs);
	
	float tMin = std::max({ tRealMins.x, tRealMins.y, tRealMins.z });
	float tMax = std::min({ tRealMaxs.x, tRealMaxs.y, tRealMaxs.z });

	if (tMax > 0 && tMax > tMin) {
		vec3 position = ray.position + tMin * direction;
		// 分量相等则说明交点在此面上，法线就加上这个方向的分量
		vec3 onMinFaceVec = -equal(position, m_minPos);
		vec3 onMaxFaceVec = equal(position, m_maxPos);
		vec3 normal = glm::normalize(onMinFaceVec + onMaxFaceVec);
		info.position = position;
		info.normal = normal;
		return true;
	}
	return false;
}

void CAABoundingBox::Draw(CShader& shader)
{
	using namespace std;
	using namespace glm;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vao);
	// glDrawArrays(GL_TRIANGLES, 0, 3);
	// glDrawArrays(GL_LINE_STRIP, 0, m_vertices.size());
	glDrawElements(GL_LINE_STRIP, m_indices.size(), GL_UNSIGNED_INT, 0);
}
