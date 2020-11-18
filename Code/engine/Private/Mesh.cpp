#include "Mesh.h"



bool Mesh::RayCast(const CRay& ray, CRayCastInfo& info)
{
	for(int i = 0; i < indices.size(); i+= 3)
	{
		if (RayCastTriangle(ray, info, vertices[i].Position, vertices[i+1].Position, vertices[i+2].Position))
		{
			return true;
		}
	}
	return false;
}