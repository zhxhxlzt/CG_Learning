#include "glm/glm.hpp"
#include "Shader.h"
#include "SourceFinder.h"
#include <string>
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