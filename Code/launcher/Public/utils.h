#include "glm/glm.hpp"
#include "Shader.h"
#include "SourceFinder.h"
#include <string>
#include "RayCast.h"


struct VertexData
{
	glm::vec3 position;
	//vec3 color;
	glm::vec2 uv;
};
VertexData vertices[] = {

	// z+
		{{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
{{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
{{ 0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}},
{{ 0.5f,  0.5f,  0.5f},  {1.0f, 1.0f}},
{{-0.5f,  0.5f,  0.5f},  {0.0f, 1.0f}},
{{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},

// x+
	{{ 0.5f,  -0.5f,  0.5f},  {0.0f, 0.0f}},
{{ 0.5f,  -0.5f, -0.5f},  {1.0f, 0.0f}},
{{ 0.5f, 0.5f, -0.5f},  {1.0f, 1.0f}},
{{ 0.5f, 0.5f, -0.5f},  {1.0f, 1.0f}},
{{ 0.5f, 0.5f,  0.5f},  {0.0f, 1.0f}},
{{ 0.5f,  -0.5f,  0.5f},  {0.0f, 0.0f}},

// z-
{{0.5f, -0.5f, -0.5f},  {0.0f, 0.0f}},
{{ -0.5f, -0.5f, -0.5f},  {1.0f, 0.0f}},
{{ -0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
{{ -0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
{{0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
{{0.5f, -0.5f, -0.5f},  {0.0f, 0.0f}},


//x-
{{-0.5f,  -0.5f,  -0.5f},  {0.0f, 0.0f}},
{{-0.5f,  -0.5f, 0.5f},  {1.0f, 0.0f}},
{{-0.5f, 0.5f, 0.5f},  {1.0f, 1.0f}},
{{-0.5f, 0.5f, 0.5f},  {1.0f, 1.0f}},
{{-0.5f, 0.5f,  -0.5f},  {0.0f, 1.0f}},
{{-0.5f,  -0.5f,  -0.5f},  {0.0f, 0.0f}},



{{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},
{{ 0.5f, -0.5f, -0.5f},  {1.0f, 1.0f}},
{{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
{{ 0.5f, -0.5f,  0.5f},  {1.0f, 0.0f}},
{{-0.5f, -0.5f,  0.5f},  {0.0f, 0.0f}},
{{-0.5f, -0.5f, -0.5f},  {0.0f, 1.0f}},

{{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}},
{{ 0.5f,  0.5f, -0.5f},  {1.0f, 1.0f}},
{{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
{{ 0.5f,  0.5f,  0.5f},  {1.0f, 0.0f}},
{{-0.5f,  0.5f,  0.5f},  {0.0f, 0.0f}},
{{-0.5f,  0.5f, -0.5f},  {0.0f, 1.0f}}
};


CShader GetBaseShader()
{
	using namespace std;
	string vs = CSourceFinder::FindShaderFullPath("base.vert");
	string fs = CSourceFinder::FindShaderFullPath("base.frag");

	return CShader(vs.data(), fs.data());
}


class CTriangleTest : CRayCastable
{
public:
	CTriangleTest()
	{
		using namespace glm;
		using namespace std;
		triangle = {
			{1, 0, 5},
			{0, 1, 5},
			{0, 0, 6}
		};

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * triangle.size(), triangle.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), NULL);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void Draw(CShader& shader);

	bool RayCast(const CRay& ray, CRayCastInfo& info) override;
	
	unsigned int vao, vbo;
	std::vector<glm::vec3> triangle;
	
};

inline void CTriangleTest::Draw(CShader& shader)
{

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	
}

inline bool CTriangleTest::RayCast(const CRay& ray, CRayCastInfo& info)
{
	return RayCastTriangle(ray, info, triangle[0], triangle[1], triangle[2]);
}
