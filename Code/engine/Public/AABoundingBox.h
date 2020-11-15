#include <vector>


#include "RayCast.h"

class CAABoundingBox : public CRayCastable
{
public:
	void Build(const std::vector<glm::vec3>& vertices);
	bool RayCast(const CRay& ray, CRayCastInfo& info) override;
	
private:
	glm::vec3 m_minPos;
	glm::vec3 m_maxPos;
	bool m_inited = false;
};