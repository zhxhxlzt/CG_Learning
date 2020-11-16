#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
class CTransform
{
public:
	CTransform() : m_position(0, 0, 0), m_rotataion(1, 0, 0, 0), m_scale(1, 1, 1) {}
	glm::vec3 m_position;
	glm::quat m_rotataion;
	glm::vec3 m_scale;

	glm::mat4 matrix();
};