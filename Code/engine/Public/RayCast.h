#pragma once
#include "glm/glm.hpp"


struct CRayCastInfo
{
	glm::vec3 position;
	glm::vec3 normal;
};


struct CRay
{
	glm::vec3 position;
	glm::vec3 direction;
};

class CRayCastable
{
public:
	virtual ~CRayCastable() = default;
	virtual bool RayCast(const CRay& ray, CRayCastInfo& info) = 0;
};