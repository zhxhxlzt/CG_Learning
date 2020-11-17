#include "Camera.h"
#include "glm/glm.hpp"
#include "math.h"


CRay Camera::GetRay(const glm::vec2& screenRatio)
{
	using namespace glm;
	vec4 v0 = { -0.5f + 0.5f * screenRatio.x, -0.5f + 0.5f * screenRatio.y, -1, 1 };
	mat4 inv_pv = inverse(GetPerspectiveProjectionMatrix() * GetViewMatrix());
	vec3 v1 = inv_pv * v0;
	vec3 v2 = v1 - Position;
	return { v1, normalize(v2) };
}
