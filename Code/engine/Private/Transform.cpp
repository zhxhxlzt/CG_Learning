#include "Transform.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

glm::mat4 CTransform::matrix()
{
	using namespace glm;
	mat4 trans(1);
	trans = scale(trans, m_scale);
	trans = mat4_cast(m_rotataion) * trans;
	trans = translate(mat4(1), m_position) * trans;
	return trans;
}
