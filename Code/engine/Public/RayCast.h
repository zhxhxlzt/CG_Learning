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


bool OnLeft(glm::vec3 AC, glm::vec3 AN, glm::vec3 AP);

bool RayCastTriangle(const CRay& ray, CRayCastInfo& info, glm::vec3 A, glm::vec3 B, glm::vec3 C);

