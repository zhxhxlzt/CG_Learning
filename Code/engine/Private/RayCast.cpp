#include "..\Public\RayCast.h"


bool OnLeft(glm::vec3 AC, glm::vec3 N, glm::vec3 AP)
{
	using namespace glm;

	return dot(cross(AC, AP), N) > 0;
}

bool RayCastTriangle(const CRay& ray, CRayCastInfo& info, glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
	using namespace glm;

	vec3 AB = B - A;
	vec3 AC = C - A;
	vec3 N = normalize(cross(AB, AC));

	float t = (dot(N, A) - dot(N, ray.position)) / dot(N, ray.direction);
	vec3 P = ray.position + ray.direction * t;

	bool left = false, right = false;

	(OnLeft(AB, N, P - A) ? left : right) = true;
	(OnLeft(C - B, N, P - B) ? left : right) = true;
	(OnLeft(A - C, N, P - C) ? left : right) = true;


	if (!(left && right))
	{
		info.position = P;
		info.normal = (dot(N, -ray.direction) > 0 ? N : -N);
		return true;
	}

	return false;
}
