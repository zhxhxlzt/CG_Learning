#include "RenderTexture.h"
#include <vector>

CRenderTexture::CRenderTexture(unsigned width, unsigned height)
{
	glGenFramebuffers(1, &m_frameBuffer);
	glGenTextures(1, &m_colorTex);
	glGenTextures(1, &m_depthTex);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
	bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

	glBindTexture(GL_TEXTURE_2D, m_colorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_colorTex, 0);

	glBindTexture(GL_TEXTURE_2D, m_depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, m_depthTex, 0);

	bool complete2 = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	struct CB {
		glm::vec3 aPos;
		glm::vec2 aUv;
	};

	std::vector<CB> cb_data = {
		{{-1, -1, -1}, {0, 0}},
		{{1, -1, -1}, {1, 0}},
		{{1, 1, -1}, {1, 1}},
		{{-1, -1, -1}, {0, 0}},
		{{1, 1, -1}, {1, 1}},
		{{-1, 1, -1}, {0, 1}},
	};

	glBufferData(GL_ARRAY_BUFFER, cb_data.size() * sizeof(CB), cb_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CB), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CB), (void*)offsetof(CB, aUv));
	glEnableVertexAttribArray(1);
}

void CRenderTexture::BindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
}

void CRenderTexture::Draw(CShader& shader)
{
	glBindVertexArray(m_vao);
	glBindTexture(GL_TEXTURE_2D, m_colorTex);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
