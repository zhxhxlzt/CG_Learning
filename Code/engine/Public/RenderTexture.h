#pragma once
#include "Shader.h"


class CRenderTexture
{
public:
	CRenderTexture(unsigned width, unsigned height);

	void BindFrameBuffer();

	void Draw(CShader& shader);

private:
	GLuint m_frameBuffer;
	GLuint m_colorTex;
	GLuint m_depthTex;
};

