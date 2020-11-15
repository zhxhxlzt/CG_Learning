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
	// todo 处理direction 某分量为0时情况
	using namespace glm;
	std::vector<float> tMins;
	std::vector<float> tMaxs;

	// x, y, z 分量分别求
	for (int i = 0; i < 3; i++)
	{
		float tMin = (m_minPos[i] - ray.position[i]) / ray.direction[i];
		float tMax = (m_maxPos[i] - ray.position[i]) / ray.direction[i];

		// 进入的tMin 必定小于出去的 tMax
		if (tMin > tMax)
		{
			std::swap(tMin, tMax);
		}
		tMins.push_back(tMin);
		tMaxs.push_back(tMax);
	}
	
	std::sort(tMins.begin(), tMins.end());
	std::sort(tMaxs.begin(), tMaxs.end());
	float tMin = *tMins.rbegin();
	float tMax = *tMaxs.begin();

	if (tMax > 0 && tMax > tMin)
	{
		info.position = ray.position + tMin * ray.direction;
		vec3 normal = vec3(0);
		
		for (int i = 0; i < 3; i++)
		{
			vec3 n = vec3(0);
			n[i] = 1;
			// 相等则说明在那个面上
			if (info.position[i] == m_maxPos[i])
			{
				normal += n;
			}
			else if (info.position[i] == m_minPos[i])
			{
				normal -= n;
			}
		}
		
		info.normal = normalize(normal);
		return true;
	}

	return false;
}
