#include "AABoundingBox.h"
#include <algorithm>
#include <array>
#include <vector>


void CAABoundingBox::Build(const std::vector<glm::vec3>& vertices)
{
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
